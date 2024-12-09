//
// Created by w2552 on 2024/11/15.
//

#ifndef VERIFYSERVER_VERIFYGRPCCLIENT_H
#define VERIFYSERVER_VERIFYGRPCCLIENT_H


#include <grpcpp/grpcpp.h>
#include "verify.grpc.pb.h"
#include "global.h"
#include "Singleton.h"
#include <queue>
using grpc::Channel;
using grpc::Status;
using grpc::ClientContext;

using message::GetVerifyReq;
using message::GetVerifyRsp;
using message::VerifyService;

class RPConPool {
public:
    RPConPool(size_t poolSize, std::string host, std::string port)
            : poolSize_(poolSize), host_(host), port_(port), b_stop_(false) {
        for (size_t i = 0; i < poolSize_; ++i) {

            std::shared_ptr<Channel> channel = grpc::CreateChannel(host+":"+port,
                                                                   grpc::InsecureChannelCredentials());

            connections_.push(VerifyService::NewStub(channel));
        }
    }

    ~RPConPool() {
        std::lock_guard<std::mutex> lock(mutex_);
        Close();
        while (!connections_.empty()) {
            connections_.pop();
        }
    }

    std::unique_ptr<VerifyService::Stub> getConnection() {
        std::unique_lock<std::mutex> lock(mutex_);
        cond_.wait(lock, [this] {
            if (b_stop_) {
                return true;
            }
            return !connections_.empty();
        });
        //如果停止则直接返回空指针
        if (b_stop_) {
            return  nullptr;
        }
        auto context = std::move(connections_.front());
        connections_.pop();
        return context;
    }

    void returnConnection(std::unique_ptr<VerifyService::Stub> context) {
        std::lock_guard<std::mutex> lock(mutex_);
        if (b_stop_) {
            return;
        }
        connections_.push(std::move(context));
        cond_.notify_one();
    }

    void Close() {
        b_stop_ = true;
        cond_.notify_all();
    }

private:
    atomic<bool> b_stop_;
    size_t poolSize_;
    std::string host_;
    std::string port_;
    std::queue<std::unique_ptr<VerifyService::Stub>> connections_;
    std::mutex mutex_;
    std::condition_variable cond_;
};

class VerifyGrpcClient:public Singleton<VerifyGrpcClient>
{
    friend class Singleton<VerifyGrpcClient>;
public:

    GetVerifyRsp GetVerifyCode(std::string email) {
        try {
            std::cout << "Starting RPC call for email: " << email << std::endl;

            ClientContext context;
            GetVerifyRsp reply;
            GetVerifyReq request;
            request.set_email(email);

            if (!stub_) {
                std::cout << "Stub is null, getting new connection from pool" << std::endl;
                stub_ = pool_->getConnection();
                if (!stub_) {
                    throw std::runtime_error("Failed to get gRPC connection");
                }
            }

            std::cout << "Making RPC call..." << std::endl;
            Status status = stub_->GetVerifyCode(&context, request, &reply);
            std::cout << "RPC call completed with status: " << status.error_message() << std::endl;

            if (status.ok()) {
                return reply;
            } else {
                std::cout << "RPC failed: " << status.error_message() << std::endl;
                reply.set_error(ErrorCodes::RPCFailed);
                return reply;
            }
        }
        catch (const std::exception& e) {
            std::cout << "Exception in GetVerifyCode: " << e.what() << std::endl;
            GetVerifyRsp error_reply;
            error_reply.set_error(ErrorCodes::RPCFailed);
            return error_reply;
        }
    }

private:
    VerifyGrpcClient();

    std::unique_ptr<VerifyService::Stub> stub_;
    std::unique_ptr<RPConPool> pool_;
};


#endif //VERIFYSERVER_VERIFYGRPCCLIENT_H

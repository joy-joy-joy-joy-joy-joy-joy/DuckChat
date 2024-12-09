#include "VerifyGrpcClient.h"
#include <grpcpp/grpcpp.h>
#include "verify.grpc.pb.h"
#include "global.h"
#include "Singleton.h"

VerifyGrpcClient::VerifyGrpcClient() {
    try {
        auto& gCfgMgr = ConfigMgr::Inst();
        std::string host = gCfgMgr["VerifyServer"]["Host"];
        std::string port = gCfgMgr["VerifyServer"]["Port"];

        std::cout << "VerifyGrpcClient init - host: " << host << ", port: " << port << std::endl;

        if (host.empty() || port.empty()) {
            throw std::runtime_error("Invalid gRPC server configuration");
        }

        // 先创建连接池
        pool_ = std::make_unique<RPConPool>(5, host, port);
        if (!pool_) {
            throw std::runtime_error("Failed to create connection pool");
        }

        // 再获取连接
        stub_ = pool_->getConnection();
        if (!stub_) {
            throw std::runtime_error("Failed to get initial gRPC connection");
        }

        std::cout << "VerifyGrpcClient initialized successfully" << std::endl;
    }
    catch (const std::exception& e) {
        std::cerr << "VerifyGrpcClient initialization failed: " << e.what() << std::endl;
        throw;
    }
}
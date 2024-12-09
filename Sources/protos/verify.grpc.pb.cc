// Generated by the gRPC C++ plugin.
// If you make any local change, they will be lost.
// source: verify.proto

#include "verify.pb.h"
#include "verify.grpc.pb.h"

#include <functional>
#include <grpcpp/support/async_stream.h>
#include <grpcpp/support/async_unary_call.h>
#include <grpcpp/impl/channel_interface.h>
#include <grpcpp/impl/client_unary_call.h>
#include <grpcpp/support/client_callback.h>
#include <grpcpp/support/message_allocator.h>
#include <grpcpp/support/method_handler.h>
#include <grpcpp/impl/rpc_service_method.h>
#include <grpcpp/support/server_callback.h>
#include <grpcpp/impl/server_callback_handlers.h>
#include <grpcpp/server_context.h>
#include <grpcpp/impl/service_type.h>
#include <grpcpp/support/sync_stream.h>
namespace message {

static const char* VerifyService_method_names[] = {
  "/message.VerifyService/GetVerifyCode",
};

std::unique_ptr< VerifyService::Stub> VerifyService::NewStub(const std::shared_ptr< ::grpc::ChannelInterface>& channel, const ::grpc::StubOptions& options) {
  (void)options;
  std::unique_ptr< VerifyService::Stub> stub(new VerifyService::Stub(channel, options));
  return stub;
}

VerifyService::Stub::Stub(const std::shared_ptr< ::grpc::ChannelInterface>& channel, const ::grpc::StubOptions& options)
  : channel_(channel), rpcmethod_GetVerifyCode_(VerifyService_method_names[0], options.suffix_for_stats(),::grpc::internal::RpcMethod::NORMAL_RPC, channel)
  {}

::grpc::Status VerifyService::Stub::GetVerifyCode(::grpc::ClientContext* context, const ::message::GetVerifyReq& request, ::message::GetVerifyRsp* response) {
  return ::grpc::internal::BlockingUnaryCall< ::message::GetVerifyReq, ::message::GetVerifyRsp, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(channel_.get(), rpcmethod_GetVerifyCode_, context, request, response);
}

void VerifyService::Stub::async::GetVerifyCode(::grpc::ClientContext* context, const ::message::GetVerifyReq* request, ::message::GetVerifyRsp* response, std::function<void(::grpc::Status)> f) {
  ::grpc::internal::CallbackUnaryCall< ::message::GetVerifyReq, ::message::GetVerifyRsp, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(stub_->channel_.get(), stub_->rpcmethod_GetVerifyCode_, context, request, response, std::move(f));
}

void VerifyService::Stub::async::GetVerifyCode(::grpc::ClientContext* context, const ::message::GetVerifyReq* request, ::message::GetVerifyRsp* response, ::grpc::ClientUnaryReactor* reactor) {
  ::grpc::internal::ClientCallbackUnaryFactory::Create< ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(stub_->channel_.get(), stub_->rpcmethod_GetVerifyCode_, context, request, response, reactor);
}

::grpc::ClientAsyncResponseReader< ::message::GetVerifyRsp>* VerifyService::Stub::PrepareAsyncGetVerifyCodeRaw(::grpc::ClientContext* context, const ::message::GetVerifyReq& request, ::grpc::CompletionQueue* cq) {
  return ::grpc::internal::ClientAsyncResponseReaderHelper::Create< ::message::GetVerifyRsp, ::message::GetVerifyReq, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(channel_.get(), cq, rpcmethod_GetVerifyCode_, context, request);
}

::grpc::ClientAsyncResponseReader< ::message::GetVerifyRsp>* VerifyService::Stub::AsyncGetVerifyCodeRaw(::grpc::ClientContext* context, const ::message::GetVerifyReq& request, ::grpc::CompletionQueue* cq) {
  auto* result =
    this->PrepareAsyncGetVerifyCodeRaw(context, request, cq);
  result->StartCall();
  return result;
}

VerifyService::Service::Service() {
  AddMethod(new ::grpc::internal::RpcServiceMethod(
      VerifyService_method_names[0],
      ::grpc::internal::RpcMethod::NORMAL_RPC,
      new ::grpc::internal::RpcMethodHandler< VerifyService::Service, ::message::GetVerifyReq, ::message::GetVerifyRsp, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(
          [](VerifyService::Service* service,
             ::grpc::ServerContext* ctx,
             const ::message::GetVerifyReq* req,
             ::message::GetVerifyRsp* resp) {
               return service->GetVerifyCode(ctx, req, resp);
             }, this)));
}

VerifyService::Service::~Service() {
}

::grpc::Status VerifyService::Service::GetVerifyCode(::grpc::ServerContext* context, const ::message::GetVerifyReq* request, ::message::GetVerifyRsp* response) {
  (void) context;
  (void) request;
  (void) response;
  return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
}


}  // namespace message


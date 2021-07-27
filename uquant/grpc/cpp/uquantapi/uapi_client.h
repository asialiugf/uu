#ifndef UBEE_UAPI_CLIENT_H
#define UBEE_UAPI_CLIENT_H
#include <iostream>
#include <memory>
#include <string>

#include <grpcpp/grpcpp.h>

#ifdef BAZEL_BUILD
#include "examples/protos/uquantapi.grpc.pb.h"
#else
#include "uquantapi.grpc.pb.h"
#endif
namespace uBEE
{
using grpc::Channel;
using grpc::ClientContext;
using grpc::Status;
using uquantapi::HelloRequest;
using uquantapi::HelloReply;
using uquantapi::Greeter;
using uquantapi::FutureApi;
using uquantapi::kBarRequest;
using uquantapi::kBar;
using uquantapi::kBarReply;

class FutureApiClient
{
public:
  FutureApiClient(std::shared_ptr<Channel> channel)
    : stub_(FutureApi::NewStub(channel)) {}

  //---------------
  kBarReply getFutureBars(const std::string& id, const std::string& start,
                    const std::string& end,int frequency);
private:
  std::unique_ptr<FutureApi::Stub> stub_;
};

} // namespace
#endif // end UBEE_UAPI_CLIENT_H

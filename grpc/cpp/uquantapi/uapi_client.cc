/*
 *
 * Copyright 2015 gRPC authors.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */

#include <iostream>
#include <memory>
#include <string>

#include <grpcpp/grpcpp.h>

#ifdef BAZEL_BUILD
#include "examples/protos/uquantapi.grpc.pb.h"
#else
#include "uquantapi.grpc.pb.h"
#endif

#include "uapi_client.h"

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

class GreeterClient
{
public:
  GreeterClient(std::shared_ptr<Channel> channel)
    : stub_(Greeter::NewStub(channel)) {}

  // Assembles the client's payload, sends it and presents the response back
  // from the server.
  std::string SayHello(const std::string& user)
  {
    // Data we are sending to the server.
    HelloRequest request;
    request.set_name(user);

    // Container for the data we expect from the server.
    HelloReply reply;

    // Context for the client. It could be used to convey extra information to
    // the server and/or tweak certain RPC behaviors.
    ClientContext context;

    // The actual RPC.
    Status status = stub_->SayHello(&context, request, &reply);

    // Act upon its status.
    if(status.ok()) {
      return reply.message();
    } else {
      std::cout << status.error_code() << ": " << status.error_message()
                << std::endl;
      return "RPC failed";
    }
  }

  std::string SayHelloAgain(const std::string& user)
  {
    // Follows the same pattern as SayHello.
    HelloRequest request;
    request.set_name(user);
    HelloReply reply;
    ClientContext context;

    // Here we can the stub's newly available method we just added.
    Status status = stub_->SayHelloAgain(&context, request, &reply);
    if(status.ok()) {
      return reply.message();
    } else {
      std::cout << status.error_code() << ": " << status.error_message()
                << std::endl;
      return "RPC failed";
    }
  }


  kBarReply getFutureBars(const std::string& id, const std::string& start,
                    const std::string& end,int frequency)
  {
    kBarRequest request;
    request.set_id(id);
    request.set_start(start);
    request.set_end(end);
    request.set_frequency(frequency);
    kBarReply reply;
    ClientContext context;

    // Here we can the stub's newly available method we just added.
    Status status = stub_->getFutureBars(&context, request, &reply);
    if(status.ok()) {
      return reply;
    } else {
      std::cout << status.error_code() << ": " << status.error_message()
                << std::endl;
      //return "RPC failed";
    }
  }


private:
  std::unique_ptr<Greeter::Stub> stub_;
};

//-------------------------------------------------------------
kBarReply FutureApiClient::getFutureBars(const std::string& id, const std::string& start,
                                   const std::string& end,int frequency)
{
  kBarRequest request;
  request.set_id(id);
  request.set_start(start);
  request.set_end(end);
  request.set_frequency(frequency);
  kBarReply reply;
  ClientContext context;

  // Here we can the stub's newly available method we just added.
  Status status = stub_->getFutureBars(&context, request, &reply);
  if(status.ok()) {
    return reply;
  } else {
    std::cout << status.error_code() << ": " << status.error_message()
              << std::endl;
    //return "RPC failed";
  }
}

//-------------------------------------------------------------

} //namespace

using namespace uBEE;

int main(int argc, char** argv)
{
  // Instantiate the client. It requires a channel, out of which the actual RPCs
  // are created. This channel models a connection to an endpoint (in this case,
  // localhost at port 50051). We indicate that the channel isn't authenticated
  // (use of InsecureChannelCredentials()).
  GreeterClient greeter(grpc::CreateChannel(
                          "localhost:50051", grpc::InsecureChannelCredentials()));
  std::string user("world");
  std::string reply = greeter.SayHello(user);
  std::cout << "Greeter received: " << reply << std::endl;

  user = "haha!!";
  reply = greeter.SayHelloAgain(user);
  std::cout << "Greeter received: " << reply << std::endl;

  std::string ID = "ru1809";
  std::string start = "ru1809";
  std::string end = "ru1809";
  int  frequency = 300;
  kBarReply ret = greeter.getFutureBars(ID,start,end,frequency);
  int size = ret.kk_size();
  std::cout << "bars num: " << size << std::endl;
  for(int i=0; i<size; i++) {
    kBar psn = ret.kk(i);
    std::cout << i+1 << " o: " << psn.o() << " h: " << psn.h() << std::endl;
  }

  FutureApiClient * api;
  api = new  FutureApiClient(grpc::CreateChannel(
                               "localhost:50051", grpc::InsecureChannelCredentials()));
  /*
  FutureApiClient futureapi(grpc::CreateChannel(
                              "localhost:50051", grpc::InsecureChannelCredentials()));
  */
  ID = "ru1809";
  start = "20180101";
  end = "20180507";
  frequency = 300;
  ret = api->getFutureBars(ID,start,end,frequency);
  size = ret.kk_size();
  std::cout << "bars num: " << size << std::endl;
  for(int i=0; i<size; i++) {
    kBar psn = ret.kk(i);
    std::cout << i+1 <<":" << psn.b() << " o: " << psn.o() << " h: " << psn.h() << std::endl;
  }

  return 0;
}

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

using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::Status;
using uquantapi::HelloRequest;
using uquantapi::HelloReply;
using uquantapi::Greeter;
using uquantapi::FutureApi;
using uquantapi::StockApi;
using uquantapi::kBarRequest;
using uquantapi::kBar;
using uquantapi::kBarReply;

// Logic and data behind the server's behavior.
class GreeterServiceImpl final : public Greeter::Service
{
  Status SayHello(ServerContext* context, const HelloRequest* request,
                  HelloReply* reply) override
  {
    std::string prefix("Hello ");
    reply->set_message(prefix + request->name());
    return Status::OK;
  }

  Status SayHelloAgain(ServerContext* context, const HelloRequest* request,
                       HelloReply* reply) override
  {
    std::string prefix("Hello again ");
    reply->set_message(prefix + request->name());
    return Status::OK;
  }

  //-----------------
  Status getFutureBars(ServerContext* context, const kBarRequest* request,
                 kBarReply* reply) override
  {
    GOOGLE_PROTOBUF_VERIFY_VERSION;

    // kBarReply reply;
    kBar* kbar;

    for(int i=0; i<10000; ++i) {
      kbar = reply->add_kk();
      kbar->set_b("09:00:00");
      kbar->set_e("10:00:00");
      kbar->set_o((double) i);
      kbar->set_h((double) i);
      kbar->set_l((double) i);
      kbar->set_c((double) i);
    }

    kbar = reply->add_kk();
    kbar->set_b("10:00:00");
    kbar->set_b("11:00:00");
    kbar->set_o(2001);
    kbar->set_h(2002);
    kbar->set_l(2003);
    kbar->set_c(2004);

    return Status::OK;

  }
  //-----------------
};

class FutureApiServiceImpl final : public FutureApi::Service
{
  //-------------------------------------------------
  Status getFutureBars(ServerContext* context, const kBarRequest* request,
                 kBarReply* reply) override
  { 
    std::cout << request->id() << " " << request->start() << " " ;
    std::cout << request->end() << " " << request->frequency() << std::endl;
    GOOGLE_PROTOBUF_VERIFY_VERSION;
    kBar* kbar;
    for(int i=0; i<10000; ++i) {
      kbar = reply->add_kk();
      kbar->set_b("09:00:00");
      kbar->set_e("10:00:00");
      kbar->set_o((double) i);
      kbar->set_h((double) i);
      kbar->set_l((double) i);
      kbar->set_c((double) i);
    }
    
    kbar = reply->add_kk();
    kbar->set_b("10:00:00");
    kbar->set_b("11:00:00");
    kbar->set_o(2001);
    kbar->set_h(2002);
    kbar->set_l(2003);
    kbar->set_c(2004);

    return Status::OK;
  }
  //-------------------------------------------------
};



void RunServer()
{
  std::string server_address("0.0.0.0:50051");
  GreeterServiceImpl   service;
  FutureApiServiceImpl futureapi;

  ServerBuilder builder;

  builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
  builder.RegisterService(&service);
  builder.RegisterService(&futureapi);

  std::unique_ptr<Server> server(builder.BuildAndStart());
  std::cout << "Server listening on " << server_address << std::endl;
  server->Wait();
}

int main(int argc, char** argv)
{
  RunServer();

  return 0;
}

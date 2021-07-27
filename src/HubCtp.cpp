#include "uBEE.h"
#include <iostream>
#include <thread>
#include <unistd.h>
#include <algorithm>
#include <vector>

namespace uBEE
{

HubCtp::HubCtp()
{
}

void HubCtp::Init()
{
  memset(URL,'\0',256);
  sprintf(URL,"3003");
  port = 3003;
  sg = &h.getDefaultGroup<uWS::SERVER>();
  cg = &h.getDefaultGroup<uWS::CLIENT>();
}

void HubCtp::S_onMessage()
{
  h.onMessage([this](uWS::WebSocket<uWS::SERVER> *ws, char *message, size_t length, uWS::OpCode opCode) {
    char tmp[256];
    memcpy(tmp, message, length);
    tmp[length] = 0;
    message[0] = '0';
    ErrLog(1000,"kkkk",1,(const char*)message,length);
    message[length-1] = 0;
    printf("Server onMessage receive: %s\n", message);
    ws->send(message, length, opCode);
    printf("Server onMessage send: %s\n", tmp);
  });
}

void HubCtp::S_onConnection()
{
  h.onConnection([this](uWS::WebSocket<uWS::SERVER> *ws, uWS::HttpRequest req) {
    //ws->send("--server--");
    std::cout <<"Server onConnection send: --server--" << std::endl;
  });
}

void HubCtp::S_onDisconnection()
{
  h.onDisconnection([this](uWS::WebSocket<uWS::SERVER> *ws, int code, char *message, size_t length) {
    std::cout << "onDisconnection !!" << std::endl;
    std::cout << message << std::endl;
  });
}

void HubCtp::S_onPong()
{
  h.onPong([this](uWS::WebSocket<uWS::SERVER> *ws, char *message, size_t length) {
    std::cout<< "ok ping\n";
  });
  h.getDefaultGroup<uWS::SERVER>().startAutoPing(2000);
}

void HubCtp::S_Listen()
{
  bool k = h.listen(port) ;
  if(!k) {
    std::cout << " listen error !!" << std::endl;
    exit(-1);
  }
}
void HubCtp::Run()
{
  h.run();
}

void  HubCtp::Start()
{
  HubCtp::S_onDisconnection();
  HubCtp::S_onConnection();
  HubCtp::S_onMessage();
  HubCtp::S_onPong();
  HubCtp::S_Listen();
  HubCtp::Run();
}

} // namespace

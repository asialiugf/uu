#include "uBEE.h"
#include "Global.h"
#include <iostream>
#include <thread>
#include <unistd.h>
#include <algorithm>
#include <vector>

namespace uBEE
{

HubBck::HubBck()
{
}

void HubBck::Init()
{
  memset(URL,'\0',256);
  sprintf(URL,"3002");
  port = 3002;
}

void HubBck::S_onMessage()
{
  h.onMessage([this](uWS::WebSocket<uWS::SERVER> *ws, char *message, size_t length, uWS::OpCode opCode) {
    switch(*((int*)message)) {
    case 0: {
      BckMsg *bckmsg = (BckMsg *)message ;
      std::cout << bckmsg->iType <<" " << bckmsg->iN << std::endl;
      std::cout << bckmsg->Uuid << std::endl;

      // --------- tb in Global.h ------------------------------------
      for(int j=0; j<7; j++) {
        int i = 0;
        while(i<SGM_NUM &&tb->TT[j].aSgms[i].iI !=-1) {
          std::cout << "----:"<< tb->TT[j].aSgms[i].cB ;
          std::cout << "----:"<< tb->TT[j].aSgms[i].cE ;
          std::cout << "----:"<< tb->TT[j].aSgms[i].iB ;
          std::cout << "----:"<< tb->TT[j].aSgms[i].iE ;
          std::cout << "----:"<< tb->TT[j].aSgms[i].iI << std::endl;
          i++;
        }
        std::cout << std::endl;
      }
      // ---- 每个交易策略一个<Uuid sTTBo*> ,放在 this->M_sTTBo 里，
      // ---- 每个sTTBo 有一个 M_FuBo,里面放置，一个个 要计算的合约 <futureID, uBEE::FuBo*>
      if(bckmsg->iN>0) {
        sTTBo *psTTBo = new sTTBo();
        this->M_sTTBo.insert(std::pair<std::string,uBEE::sTTBo *>(std::string(bckmsg->Uuid), psTTBo));
        for(int i=0;i<bckmsg->iN;++i) {
            uBEE::FuBo *fubo = new uBEE::FuBo(bckmsg->fubo[i].InstrumentID,tb, nullptr);
            psTTBo->M_FuBo.insert(std::pair<std::string,uBEE::FuBo*>(bckmsg->fubo[i].InstrumentID, fubo));
        }
      }

    break;
  }
  case 1:
    ws->send(message, length, opCode);
      break;
    }
  });
}

void HubBck::S_onConnection()
{
  h.onConnection([this](uWS::WebSocket<uWS::SERVER> *ws, uWS::HttpRequest req) {
    //ws->send("--server--");
    //todo ----
    // save ws....
    long user_data = (long) ws->getUserData();
    std::cout <<"BackTest Server onConnection :"<< user_data << "pid:" << getpid() << std::endl;
  });
}

void HubBck::S_onDisconnection()
{
  h.onDisconnection([this](uWS::WebSocket<uWS::SERVER> *ws, int code, char *message, size_t length) {
    std::cout << "onDisconnection !!" << std::endl;
    std::cout << message << std::endl;
  });
}

void HubBck::S_onPong()
{
  h.onPong([this](uWS::WebSocket<uWS::SERVER> *ws, char *message, size_t length) {
    std::cout<< "ok ping\n";
  });
  h.getDefaultGroup<uWS::SERVER>().startAutoPing(2000);
}

void HubBck::S_Listen()
{
  //bool k = h.listen(port) ;
  bool k = h.listen(port, nullptr, uS::REUSE_PORT);
  if(!k) {
    std::cout << " listen error !!" << std::endl;
    exit(-1);
  }
}
void HubBck::Run()
{
  h.run();
}

void  HubBck::Start()
{
  HubBck::S_onDisconnection();
  HubBck::S_onConnection();
  HubBck::S_onMessage();
  HubBck::S_onPong();
  HubBck::S_Listen();
  //HubBck::Run();
}

} // namespace

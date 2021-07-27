#ifndef UBEE_HUBSIM_H
#define UBEE_HUBSIM_H

#include <uWS/uWS.h>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <functional>
#include <vector>
#include <queue>

namespace uBEE
{

struct HubSim {
  char URL[256];
  int port;
  uWS::Hub   h;
  uWS::Group<uWS::SERVER> * sg;
  uWS::Group<uWS::CLIENT> * cg;
  //h.getDefaultGroup<uWS::SERVER>().broadcast(brMsg, brMsgLength, opCode);
public:
  HubSim();
  void Init();
  void Start();
private:
  void S_onDisconnection();
  void S_onConnection();
  void S_onMessage();
  void S_onPong();
  void S_Listen();
  void Run();
};

} //namespace

#endif // UBEE_HUBSIM_H

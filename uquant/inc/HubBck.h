#ifndef UBEE_HUBBCK_H
#define UBEE_HUBBCK_H

#include <uWS/uWS.h>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <functional>
#include <vector>
#include <queue>
#include <map>

namespace uBEE
{

//--- message ----------- from strategy to HubBck --------------------
struct BckFu {
  char            InstrumentID[31];
  int             iP[50] ;           //period  如果没有用到 frequency 必须设置为-1 要小于0
};

struct BckMsg {
  int     iType ;                 // type :  0:初始请求， 1:请传下一个数据
  char    DayB[9];                // 测试起始日期
  char    DayE[9];                // 测试结束日期
  char    Uuid[33];               // 每个策略一个
  int     iN ;                 // number of BckFu below !!
  BckFu   fubo[256];           // 一个策略中最多允许256个合约同时处理。
};
//--- message ----------- from strategy to HubBck --------------------
// strategy block ----
struct sTTBo {
  std::map<std::string,uBEE::FuBo *>    M_FuBo;  // < futureID, fubo *>
};
//---
struct HubBck {
  char URL[256];
  int port;
  uWS::Hub   h;
  std::map<std::string,uBEE::sTTBo *>   M_sTTBo;   // < Uuid , sTTBo *>
public:
  HubBck();
  void Init();
  void Start();
  void Run();
private:
  void S_onDisconnection();
  void S_onConnection();
  void S_onMessage();
  void S_onPong();
  void S_Listen();
};

} //namespace

#endif // UBEE_HUBBCK_H

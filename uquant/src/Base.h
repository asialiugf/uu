#ifndef UBEE_BASE_H
#define UBEE_BASE_H

/* 暂时不用grpc
#include "../grpc/cpp/uquantapi/uapi_client.h"
*/
#include <uBEE.h>
//#include "Bars.h"
#include "TdFuBo.h"
#include <uWS/uWS.h>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <functional>
#include <vector>
#include <queue>

namespace uBEE
{

#define ON_TICK      '1'
#define ON_BARS      '2'
#define ON_ERROR     '3'

// sFuBo storage for  future and periods! 
// the memory image:
//  "ru1805"
//  iP[0] = -1;
//  ip[1] = -1;
//  ip[2] = -1;
//  ip[3] = -1;
//  ip[4] = 5;     // index 4 === period is 5 seconds.
//  ...
//  ip[x] = 60;    // index x === period is 60 seconds.
//  ...
//  ip[49] = -1;
// mainhub.onMessage will set bars[0] bars[1] 
// bars[0] ==> ip[4] = 5;
// bars[1] ==> ip[x] = 60; 
// ip[n] noused will be set to  -1 ;
// refer to FuInit() ! | index 0 for tick. index 1----30 basic periods.  index 31-49 for custom define .
/*
struct sFuBo {
  char    InstrumentID[31];
  char    ID2[3] ;
  int     iP[50] ;  //period
  double  mDif ;   // 变动差 比如 橡胶变化是5元
  double  mLot ;   // 每手收益 比如 一手橡胶 是50元
  double  mOP ;    // money for open position 开仓手续费
  double  mCP ;    // money for close position 平仓手续费
public:
  sFuBo();
};
*/

struct Base {
  int Mode;                       // 1:null 2: back test, 3: ctp , 4: simulation
  // FutureApiClient                * api; 暂时不用grpc
  std::map<std::string,sFuBo *>    MFuBo;    // map <"ru1805",struct sFuBo> 记录策略要用到的 合约,和用户在策略主进程中定义的
  char    DayB[9];                // 测试起始日期
  char    DayE[9];                // 测试结束日期
  char    Uuid[33];               // std::map< std::string, std::vector<int> > fuMap ; 一致，通过 BB->FuInit(&fuMap); 来初始化。

  // -------- 下面的变量只记录当前收到的 future的信息 --------------------
  // 如果是多合约策略，要在其它的地方保存更完整的每个合约的基础信息 ------ 
  sData  DT;                        // recieving DaTa for  Bars.cpp( SG.broadcast() ) !!
  sData *data;                      // recieving data from mainhub.onMessage 
  sTick  TK;                        // recieving DaTa for  Bars.cpp( SG.broadcast() ) !!
  sTick *tick;                      // recieving data from mainhub.onMessage 
  char  *InstrumentID;              // point to data->InstrumentID
  char  *TradingDay;                // point to data->TradingDay
  char  *ActionDay;                 // point to data->ActionDay
  sKbar *bars[50] ;                 // current bars! mainhub.onMessage will set bars[0] bars[1] ... and send it to onBars() !!
  sFuBo *fu ;                      // current future block for strategy !! more fu, refer to M_Fu ......

  uWS::Hub mainHub;   				// main     Hub callback for onTick() onBars() ...
  uWS::WebSocket<uWS::CLIENT> *cw;  // (mainHub) client to web server
  uWS::WebSocket<uWS::CLIENT> *cd;  // (mainHub) client to data server
  uWS::WebSocket<uWS::CLIENT> *c_bck;  // (mainHub) client to data server HubBck   back test
  uWS::WebSocket<uWS::CLIENT> *c_ctp;  // (mainHub) client to data server HubCtp   ctp
  uWS::WebSocket<uWS::CLIENT> *c_sim;  // (mainHub) client to data server HubSim   simulation
  uWS::WebSocket<uWS::CLIENT> *ct;  // (mainHub) client to trading server
  uWS::WebSocket<uWS::CLIENT> *ca;  // (assiHub) assitant client to data server for getTick,getBar ...
  std::vector<uWS::WebSocket<uWS::CLIENT>*>  cs;

  std::function<void()> onDailyHandler;
  std::function<void()> onWeeklyHandler;
  std::function<void()> onMonthlyHandler;

  std::function<void()> onInitHandler;
  std::function<void(sKbar *[],int)> onBarsHandler;
  std::function<void(sTick *)> onTickHandler;

public:
  Base();
  void Init();
  void Run();
  void Stop();
  void Restart();
  void Pause();
  void Continue();
  
  void onDaily(std::function<void()> handler);
  void onWeekly(std::function<void()> handler);
  void onMonthly(std::function<void()> handler);

  void onInit(std::function<void()> handler);
  void onTick(std::function<void(sTick *)> handler);
  void onBars(std::function<void(sKbar *[],int)> handler);

  void onMessageInit();
  // ------------------------------- API ---------------------------------
  void FuInit(const std::map<std::string,std::vector<int>> *M);
  int GetFrequencyIdx(int f);

private:
  int BckMsgInit(BckMsg *bck_msg);
  void BckNextData();
};

} // namespace ---

#endif // UBEE_BASE_H

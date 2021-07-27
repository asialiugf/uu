//#include "../ctp/ThostFtdcMdApi.h"
#include "ErrLog.h"
#include "FuList.h"
#include "MdSpi.h"
#include "Bars.h"
#include "Global.h"
//#include "Psqlpool.h"
//#include "PsqlFunc.h"
#include <uWS/uWS.h>
#include <string.h>
#include <iostream>
#include <ctime>
#include <cstdio>
#include <map>

namespace uBEE
{
using namespace std;

//------- from MkCtp.cpp ----------------begin ----------
extern CThostFtdcMdApi* pUserApi;
extern uWS::Group<uWS::SERVER> * sg;
//extern uBEE::TmBo         *tmbo;
//------- from MkCtp.cpp ---------------- end ----------

TThostFtdcBrokerIDType  BROKER_ID = "9999";         // 经纪公司代码
TThostFtdcInvestorIDType INVESTOR_ID = "059979";        // 投资者代码
TThostFtdcPasswordType  PASSWORD = "123456";            // 用户密码

char *ppInstrumentID[FUTURE_NUMBER];                             // 期货ID
int iInstrumentID;                                      // 订阅的期货数量
int iRequestID;

std::map<std::string,uBEE::FuBo *>    M_CtpFuBo;         // 每个期货一个 FuBo，future block 构成一个MAP

//std::shared_ptr<uBEE::DBPool> dbpool;
//uBEE::TradingTime             *tt;
uBEE::FuList                  *fl;
//uBEE::TmBo        *tmbo;
//------- ----------------- ----------------  ----------

CMdSpi::CMdSpi()
{
  Init();
}

void CMdSpi::Init()
{
  // 获取日期 .......................................................
  int y,m,d;
  time_t rawtime;
  struct tm *ptminfo;
  time(&rawtime);
  ptminfo = localtime(&rawtime);

  printf("current: %02d-%02d-%02d %02d:%02d:%02d\n",
         ptminfo->tm_year + 1900, ptminfo->tm_mon + 1, ptminfo->tm_mday,
         ptminfo->tm_hour, ptminfo->tm_min, ptminfo->tm_sec);

  y = ptminfo->tm_year + 1900 ;
  m = ptminfo->tm_mon + 1 ;
  d = ptminfo->tm_mday ;

  // 生成 future list ...............................................
  fl = new uBEE::FuList();
  int rtn = fl->Init(y,m,d);
  if(rtn < 0) {
    std::cout << "fl->Init error!!!!!!" << std::endl;
    exit(-1);
  }

  // 初始化 期货列表 ... ppInstrumentID .................begin.......
  for(int i = 0; i< FUTURE_NUMBER; i++) {
    ppInstrumentID[i] = nullptr ;
  }
  for(int i = 0; i< FUTURE_NUMBER; i++) {
    if(fl->pc_futures[i] == nullptr) {
      iInstrumentID = i;                         // 最大订阅合约数
      break ;
    }
    ppInstrumentID[i] = fl->pc_futures[i] ;
  }

  // ---- for testing begin ---------------------------------------
  for(int i = 0; i< FUTURE_NUMBER; i++) {
    if(ppInstrumentID[i] == nullptr) {
      break ;
    }
    std::cout << "=============:" << ppInstrumentID[i] << std::endl;
    sprintf(ca_errmsg,"99999==========: %s",ppInstrumentID[i]) ;
    uBEE::ErrLog(1000,ca_errmsg,1,0,0);
  }
  // ---- for testing end  ---------------------------------------

  // ---- for testing begin ---------------------------------------
  std::cout << "999999999999999999999\n" ;
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
  std::cout << "999999999999999999999\n" ;
  // ---- for testing end  ---------------------------------------


  //dbpool = std::make_shared<uBEE::DBPool>();

  // ...... 每个 future 生成一个 future block ( new uBEE::FuBo )
  for(int i = 0; i< FUTURE_NUMBER; i++) {
    if(fl->pc_futures[i] == nullptr) {
      break ;
    }
    //fb->dbpool = dbpool;
    //uBEE::createTickTable(dbpool,fl->pc_futures[i]);
    // !!! map 做为成员变量有问题，所以改成了全局变量。

    uBEE::FuBo *fubo = new uBEE::FuBo(fl->pc_futures[i],tb, sg);  // every future has a fubo: future block !
    M_CtpFuBo.insert(std::pair<std::string,uBEE::FuBo*>(fl->pc_futures[i], fubo));

  }
  //exit(0); // charmi
}

void CMdSpi::set_SG(uWS::Group<uWS::SERVER> * sg)
{
  m_SG = sg ;
}

void CMdSpi::set_InstrumentID(char *pp[])
{
  // 需要修改！
  //m_ppInstrumentID = pp ;
}

void CMdSpi::OnRspError(CThostFtdcRspInfoField *pRspInfo,
                        int nRequestID, bool bIsLast)
{
  cerr << "--->>> "<< "OnRspError" << endl;
  IsErrorRspInfo(pRspInfo);
}

void CMdSpi::OnFrontDisconnected(int nReason)
{
  cerr << "--->>> " << "OnFrontDisconnected" << endl;
  cerr << "--->>> Reason = " << nReason << endl;
}

void CMdSpi::OnHeartBeatWarning(int nTimeLapse)
{
  cerr << "--->>> " << "OnHeartBeatWarning" << endl;
  cerr << "--->>> nTimerLapse = " << nTimeLapse << endl;
}

void CMdSpi::OnFrontConnected()
{
  cerr << "--->>> " << "OnFrontConnected" << endl;
  // 用户登录请求
  ReqUserLogin();
}

void CMdSpi::ReqUserLogin()
{
  std::cout << " kkkkkkkkkkkkkkkkkk " << std::endl;
  uBEE::ErrLog(1000,"kkkkkkkkkkkkkkkkkkkkkkk",1,0,0);
  CThostFtdcReqUserLoginField req;
  memset(&req, 0, sizeof(req));
  strcpy(req.BrokerID, BROKER_ID);
  strcpy(req.UserID, INVESTOR_ID);
  strcpy(req.Password, PASSWORD);
  int iResult = pUserApi->ReqUserLogin(&req, ++iRequestID);
  cerr << "--->>> 发送用户登录请求: " << ((iResult == 0) ? "成功" : "失败") << endl;
  /*
  while(1) {
    sg->broadcast("hahaha", 7, uWS::OpCode::TEXT);
    sleep(1);
  }
  */
}

void CMdSpi::OnRspUserLogin(CThostFtdcRspUserLoginField *pRspUserLogin,
                            CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
  cerr << "--->>> " << "OnRspUserLogin" << endl;
  if(bIsLast && !IsErrorRspInfo(pRspInfo)) {
    ///获取当前交易日
    cerr << "--->>> 获取当前交易日 = " << pUserApi->GetTradingDay() << endl;
    // 请求订阅行情
    SubscribeMarketData();
  }
}

void CMdSpi::SubscribeMarketData()
{
  //int iResult = pUserApi->SubscribeMarketData(ppInstrumentID, iInstrumentID);
  int iResult = pUserApi->SubscribeMarketData(fl->pc_futures, iInstrumentID);
  cerr << "--->>> 发送行情订阅请求: " << ((iResult == 0) ? "成功" : "失败") << endl;
}

void CMdSpi::OnRspSubMarketData(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
  cout << "OnRspSubMarketData instrumentId is " << pSpecificInstrument->InstrumentID << endl;
}

void CMdSpi::OnRspUnSubMarketData(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
  cerr << "OnRspUnSubMarketData" << endl;
}

void CMdSpi::OnRtnDepthMarketData(CThostFtdcDepthMarketDataField *tick)
{
  /*
  time_t t = time(0);
  char currentTime[25];
  strftime(currentTime, sizeof(currentTime), "%Y/%m/%d %X",localtime(&t));


  map<std::string,uBEE::FuBlock>::iterator it;
  it=FuBlockMap.find(tick->InstrumentID);
  if(it==FuBlockMap.end())
    std::cout<<"we do not find :"<< tick->InstrumentID <<std::endl;
  else {
    cout << "DateTime " << currentTime <<
         " ID is " << tick->InstrumentID <<
         " Lastprice is " << tick->LastPrice <<
         " time_type is " << it->second.Block.i_hour_type << endl;
    see_handle_bars(&(it->second.Block), tick);
    see_handle_bars(&(it->second.Block), tick);
  }
  */

  map<std::string,uBEE::FuBo*>::iterator iter;
  iter=M_CtpFuBo.find(tick->InstrumentID);
  if(iter==M_CtpFuBo.end())
    std::cout<<"we do not find :"<< tick->InstrumentID <<std::endl;
  else {
    HandleTick(iter->second,tick,SEND_SAVE_ALL);
  }
}

bool CMdSpi::IsErrorRspInfo(CThostFtdcRspInfoField *pRspInfo)
{
  // 如果ErrorID != 0, 说明收到了错误的响应
  bool bResult = ((pRspInfo) && (pRspInfo->ErrorID != 0));
  if(bResult)
    cerr << "--->>> ErrorID=" << pRspInfo->ErrorID << ", ErrorMsg=" << pRspInfo->ErrorMsg << endl;
  return bResult;
}
} //end namespace

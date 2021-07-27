#include "../ctp/ThostFtdcMdApi.h"
#include "FuList.h"
#include "Bars.h"
//#include "Psqlpool.h"
#include <uWS/uWS.h>


namespace uBEE
{

class CMdSpi : public CThostFtdcMdSpi
{
  uWS::Group<uWS::SERVER> * m_SG;
  uBEE::FuList *fl;
  // map做为成员变量，初始化 没有搞定，所以改成全局变量了。
  //std::map<std::string,uBEE::FuBlock *> FuBlockMap;
  //std::shared_ptr<uBEE::DBPool> dbpool;
  int x ;
public:
  CMdSpi();
  void Init() ;
  void set_SG(uWS::Group<uWS::SERVER> * sg) ;
  void set_InstrumentID(char *pp[]) ;

  ///错误应答
  virtual void OnRspError(CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

  ///当客户端与交易后台通信连接断开时，该方法被调用。当发生这个情况后，API会自动重新连接，客户端可不做处理。
  ///@param nReason 错误原因
  ///        0x1001 网络读失败
  ///        0x1002 网络写失败
  ///        0x2001 接收心跳超时
  ///        0x2002 发送心跳失败
  ///        0x2003 收到错误报文
  virtual void OnFrontDisconnected(int nReason);

  ///心跳超时警告。当长时间未收到报文时，该方法被调用。
  ///@param nTimeLapse 距离上次接收报文的时间
  virtual void OnHeartBeatWarning(int nTimeLapse);

  ///当客户端与交易后台建立起通信连接时（还未登录前），该方法被调用。
  virtual void OnFrontConnected();

  ///登录请求响应
  virtual void OnRspUserLogin(CThostFtdcRspUserLoginField *pRspUserLogin,	CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

  ///订阅行情应答
  virtual void OnRspSubMarketData(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

  ///取消订阅行情应答
  virtual void OnRspUnSubMarketData(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

  ///深度行情通知
  virtual void OnRtnDepthMarketData(CThostFtdcDepthMarketDataField *pDepthMarketData);

private:
  void ReqUserLogin();
  void SubscribeMarketData();
  bool IsErrorRspInfo(CThostFtdcRspInfoField *pRspInfo);
public:
  char *m_ppInstrumentID[];
};
}

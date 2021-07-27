#include "../ctp/ThostFtdcMdApi.h"
#include <uWS/uWS.h>
#include "MdSpi.h"
#include "Bars.h"
#include "FuList.h"
#include <map>
#include <string>
#include <iostream>
#include <cstdio>
#include <ctime>

namespace uBEE
{

char FRONT_ADDR[] = "tcp://180.168.146.187:10011";		// 前置地址
//--- define for MdSpi.cpp -------begin--
CThostFtdcMdApi* pUserApi; // UserApi对象
uWS::Group<uWS::SERVER> * sg;
//--- define for MdSpi.cpp -------end--

void MkCtp(uWS::Group<uWS::SERVER> * new_sg)
{
  sg = new_sg;

  // ......  创建 pUserApi 并初始化................................
  pUserApi = CThostFtdcMdApi::CreateFtdcMdApi();

  // ......  创建 pUserSpi 并初始化................................
  //CThostFtdcMdSpi* pUserSpi = new CMdSpi();
  CMdSpi * pUserSpi = new CMdSpi();
  //pUserSpi->Init(1001);
  pUserSpi->set_SG(new_sg);

  // ..............................................................
  pUserApi->RegisterSpi(pUserSpi);						// 注册事件类
  pUserApi->RegisterFront(FRONT_ADDR);					// connect
  pUserApi->Init();
  pUserApi->Join();
  pUserApi->Release();
}

} // end namespace uBEE

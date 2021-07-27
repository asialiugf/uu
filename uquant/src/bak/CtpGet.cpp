#include "MdSpi.h"
#include <iostream>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include "ThostFtdcMdApi.h"
#include <future.h>

/*
*   FUTURE_NUMBER ：表示有多少个期货。
*   pc_futures 是指针数组，经过  rtc = see_futures_init(pc_futures,ca_futures) ;
*   进行初始化，see_futures_init 会从../../etc/tbl/see_future_table文件里读出有哪些期货，
*   并将这些期货初始化到 ca_futures和pc_futures中
*/

using namespace std;

char ca_errmsg[256] ;
char ca_jsonfile[] = "../../etc/json/see_brokers.json" ;

CThostFtdcMdApi* pUserApi1;                             // UserApi对象

char FRONT_ADDR4[] = "tcp://116.228.246.81:41213";		// 前置地址  招商期货
TThostFtdcBrokerIDType	    BROKER_ID       = "8070";			// 经纪公司代码 招商期货
TThostFtdcInvestorIDType    INVESTOR_ID     = "********";		// 投资者代码
TThostFtdcPasswordType      PASSWORD        = "******";		    // 用户密码

int     iInstrumentID = 3;									// 行情订阅数量
int     iRequestID = 1;                                     // 请求编号

cJSON   *json ;
cJSON   *broker ;
cJSON   *ip ;

int see_ctpget()
{
    int rtc;

    iInstrumentID = 100;
    pUserApi1 = CThostFtdcMdApi::CreateFtdcMdApi("1.con");          // 创建UserApi
    CThostFtdcMdSpi* pUserSpi1 = new CMdSpi();                      // 创建UserSpi  可以创建多个

    pUserApi1->RegisterSpi(pUserSpi1);                              // 注册事件类
    pUserApi1->RegisterFront(FRONT_ADDR4);                          // connect

    pUserApi1->Init();
    pUserApi1->Join();
    return 0;
}

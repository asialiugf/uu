#include "MdSpi.h"
#include <iostream>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include "ThostFtdcMdApi.h"
#include <future.h>

/*
*   FUTURE_NUMBER ����ʾ�ж��ٸ��ڻ���
*   pc_futures ��ָ�����飬����  rtc = see_futures_init(pc_futures,ca_futures) ;
*   ���г�ʼ����see_futures_init ���../../etc/tbl/see_future_table�ļ����������Щ�ڻ���
*   ������Щ�ڻ���ʼ���� ca_futures��pc_futures��
*/

using namespace std;

char ca_errmsg[256] ;
char ca_jsonfile[] = "../../etc/json/see_brokers.json" ;

CThostFtdcMdApi* pUserApi1;                             // UserApi����

char FRONT_ADDR4[] = "tcp://116.228.246.81:41213";		// ǰ�õ�ַ  �����ڻ�
TThostFtdcBrokerIDType	    BROKER_ID       = "8070";			// ���͹�˾���� �����ڻ�
TThostFtdcInvestorIDType    INVESTOR_ID     = "********";		// Ͷ���ߴ���
TThostFtdcPasswordType      PASSWORD        = "******";		    // �û�����

int     iInstrumentID = 3;									// ���鶩������
int     iRequestID = 1;                                     // ������

cJSON   *json ;
cJSON   *broker ;
cJSON   *ip ;

int see_ctpget()
{
    int rtc;

    iInstrumentID = 100;
    pUserApi1 = CThostFtdcMdApi::CreateFtdcMdApi("1.con");          // ����UserApi
    CThostFtdcMdSpi* pUserSpi1 = new CMdSpi();                      // ����UserSpi  ���Դ������

    pUserApi1->RegisterSpi(pUserSpi1);                              // ע���¼���
    pUserApi1->RegisterFront(FRONT_ADDR4);                          // connect

    pUserApi1->Init();
    pUserApi1->Join();
    return 0;
}

#include <iostream>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include "ThostFtdcMdApi.h"
#include <future.h>
#include "MdSpi.h"


/*
*   FUTURE_NUMBER ����ʾ�ж��ٸ��ڻ���
*   pc_futures ��ָ�����飬����  i_rtn = see_futures_init(pc_futures,ca_futures) ;
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
    int i_rtn;


    if(gp_conf->i_save_tick_only != 1) {
        pthread_mutex_init(&gp_conf->mutex_dat, NULL);
        pthread_mutex_init(&gp_conf->mutex_bar, NULL);
        pthread_cond_init(&gp_conf->cond_dat, NULL);
        pthread_cond_init(&gp_conf->cond_bar, NULL);
        i_rtn = pthread_create(&gp_conf->p_dat, NULL, see_pthread_dat, gp_conf);
        if(i_rtn == -1) {
            see_err_log(0,0,"see_ctpget(): pthread_create() error!  errno=%d\n",errno);
            return -1;
        }
    }
    //pthread_create(&gp_conf->p_bar, NULL, see_pthread_bar, &gp_conf);
    //sleep(1) ;


    iInstrumentID = 100;
    pUserApi1 = CThostFtdcMdApi::CreateFtdcMdApi("1.con");          // ����UserApi
    CThostFtdcMdSpi* pUserSpi1 = new CMdSpi();                      // ����UserSpi  ���Դ������


    pUserApi1->RegisterSpi(pUserSpi1);                              // ע���¼���
    pUserApi1->RegisterFront(FRONT_ADDR4);                          // connect

    pUserApi1->Init();
    cout << "after Init() !" << endl;

    //pUserApi1->SubscribeMarketData(pc_futures, iInstrumentID);
    //cout << "  pUserApi1->SubscribeMarketData !!!" << endl ;

    pthread_t pthID = pthread_self();
    cerr << "================main 02 =============================  pthId:" << pthID << endl;

    pUserApi1->Join();
    cout << "after Join() !" << endl;

    pthID = pthread_self();
    cerr << "03 pthId:" << pthID << endl;
    return 0;
}

int see_fork_ctpget()
{
    int rc;
    int pid;
    int i = 0;
    char ca_futures[1024];
    pid = fork();
    switch(pid) {
    case -1:
        return -1;

    case 0:
        pid = getpid();
        see_memzero(ca_futures,1024);
        for(i=0; i<gp_conf->i_future_num; i++) {
            strcat(ca_futures,gp_conf->ca_futures[i]) ;
            strcat(ca_futures," ");
        }
        setproctitle("%s %s [%s]", "future.x :", "ctpget",ca_futures);

        if(gp_conf->i_save_tick_only == 1) {
            see_ctpget() ;
        } else {

            /*
             *  see_ctpget() ��ͨ�� zmq �� ohlc���� sttrun() �������ʼ����
             *  ÿ��future.x ֻ����һ��ctpget �����Կ�����ȫ�ֵ� gp_conf������ zmq_ctxsock��
             *  ���Ҫ���ɶ��ctpget�� �����ж�� future.x ����, ÿ��future.x ���Լ������� gp_conf
            */
            if(gp_conf->send_on) {
                rc = see_zmq_pub_init(gp_conf->ca_zmq_pub_url,&gp_conf->pub_ctxsock);
                if(rc<0) {
                    see_err_log(0,0,"see_fork_ctpget(): see_zmq_pub_init() error !");
                    exit(-1);
                }
                //rc = see_zmq_sub_init(gp_conf->ca_zmq_sub_url,&gp_conf->sub_ctxsock,"test01");
            }

            if(gp_conf->stt_on) {
                //see_sttrun_init() ;
                /* �ĸ�future ���ĸ� ���ԣ� */
            }

            see_ctpget() ;

            if(gp_conf->send_on) {
                rc = see_zmq_pub_close(&gp_conf->pub_ctxsock);
                if(rc<0) {
                    see_err_log(0,0,"see_fork_ctpget(): see_zmq_pub_init() error !");
                    exit(-1);
                }
                //see_zmq_sub_close(&gp_conf->sub_ctxsock);
            }
        }

        break;

    default:
        break;
    }
    return 0;
}

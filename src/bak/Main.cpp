#include "uBEE.h"
#include "Index.h"
#include <thread>
#include <unistd.h>
#include <iostream>

using namespace uBEE;
int main()
{

  struct  timeval start;
  struct  timeval end;


  //31  32   33   34   35
  const int fr[5] = {19,14401,9900,350,6600};

  std::map< std::string, std::vector<int> > fuMap ;
  fuMap["xu1801"] = {0,60,300,3600};
  fuMap["ru1805"] = {5,60,300,3600};
  fuMap["xu1807"] = {60,19,300,3600};
  fuMap["zz1805"] = {5,15,30,60,300,3600,14401};
  fuMap["ag1809"] = {5,15,30,60,300,3600,14401};

  //-------------------- 变量定义 -----------------------------------
  uBEE::Base *BB = new uBEE::Base();
  BB->Mode = 4;
  BB->FuInit(&fuMap,&fr[0],5);

  MY_OHLC s5S ;
  MY_OHLC s1F ;
  MY_OHLC s5F ;

  //-------------------- 变量定义 -----------------------------------
  MY_KDJ s5S_1 ;
  MY_KDJ s5S_2 ;
  MY_KDJ s5S_3 ;
  MY_KDJ s5S_4 ;
/*
  //--- KDJ 1 ---
  std::vector<double>    R1(100000,SEE_NULL) ;
  std::vector<double>    K1(100000,SEE_NULL) ;
  std::vector<double>    D1(100000,SEE_NULL) ;
  std::vector<double>    J1(100000,SEE_NULL) ;
  double                 prH1 = SEE_NULL ;
  double                 prL1 = SEE_NULL ;
  int                    prF1 = SEE_NULL ;

  //--- KDJ 2 ---
  std::vector<double>    R1(100000,SEE_NULL) ;
  std::vector<double>    K1(100000,SEE_NULL) ;
  std::vector<double>    D1(100000,SEE_NULL) ;
  std::vector<double>    J1(100000,SEE_NULL) ;
  double                 prH1 = SEE_NULL ;
  double                 prL1 = SEE_NULL ;
  int                    prF1 = SEE_NULL ;

  //--- KDJ 3 ---
  std::vector<double>    R1(100000,SEE_NULL) ;
  std::vector<double>    K1(100000,SEE_NULL) ;
  std::vector<double>    D1(100000,SEE_NULL) ;
  std::vector<double>    J1(100000,SEE_NULL) ;
  double                 prH1 = SEE_NULL ;
  double                 prL1 = SEE_NULL ;
  int                    prF1 = SEE_NULL ;

  //--- KDJ 4 ---
  std::vector<double>    R1(100000,SEE_NULL) ;
  std::vector<double>    K1(100000,SEE_NULL) ;
  std::vector<double>    D1(100000,SEE_NULL) ;
  std::vector<double>    J1(100000,SEE_NULL) ;
  double                 prH1 = SEE_NULL ;
  double                 prL1 = SEE_NULL ;
  int                    prF1 = SEE_NULL ;
*/

  //-------------------- 变量定义 -----------------------------------

  exit(0);

  gettimeofday(&start,NULL);
  for(int i=0; i<100; i++) {
    BB->getFutureTick("20170101", "20180101");
  }
  gettimeofday(&end,NULL);
  unsigned  long diff = 1000000 * (end.tv_sec-start.tv_sec)+ end.tv_usec-start.tv_usec;
  diff = diff/10000;
  printf("thedifference is %ld\n",diff);
  //uBEE::ErrLog(1000,"kkkk",1,(const char*)&diff,sizeof(unsigned  long));
  //uBEE::ErrLog(1000,"notttttttttttttt",1,0,0);

  std::cout << " end getTick() !!\n";

  int aa = 1009;

  //-------------------- 变量定义 -----------------------------------
  BB->onTick([&aa,&BB](sTick *tick) {
    //sTick * tick = (sTick *)data ;
    if(tick->iX == 0) {
      snprintf(ca_errmsg,1000,"T:%s %s %06d S:%d A:%s H:%g L:%g LP:%g AP:%g AV:%d BP:%g BV:%d OI:%g V:%d\n",
               BB->TradingDay,   tick->UpdateTime,
               tick->UpdateMillisec*1000, 0,            BB->ActionDay,
               tick->HighestPrice, tick->LowestPrice,   tick->LastPrice,
               tick->AskPrice1,    tick->AskVolume1,
               tick->BidPrice1,    tick->BidVolume1,
               tick->OpenInterest, tick->Volume);
      std::cout << ca_errmsg ;
    }
    /*
     for(int i = 0; i< KBuf->iN ; ++i) {
       std::cout << KBuf->KK[i].cK << std::endl;
     }
     */
    // usleep(1000000);
    //std::cout << aa << std::endl;
    //aa = 5990;
    //std::cout << aa << std::endl;
  });

  BB->onBars([&aa,&BB](sKbar * bar[], int len) {
    //sKbar * bar = (sKbar *)data ;
    //printf("%s %s %s\n",b->InstrumentID,b->TradingDay,b->ActionDay);
    for(int i=0; i<len; ++i) {
      if(bar[i]->iF != 0) {
        snprintf(ca_errmsg,ERR_MSG_LEN,"%s T:%s A:%s %s--%s O:%g H:%g L:%g C:%g V:%d vsam:%d\n",
                 BB->InstrumentID, BB->TradingDay, BB->ActionDay,
                 bar[i]->cB, bar[i]->cE,
                 bar[i]->o, bar[i]->h, bar[i]->l, bar[i]->c,
                 bar[i]->v, bar[i]->vsum) ;
        std::cout << ca_errmsg ;
      }
    }
    //usleep(1000000);

    //message[len] = 0;
    //std::cout<<" I am in onBars !"<<std::endl;
    //std::cout<<message<<std::endl;
    //std::cout << aa << std::endl;
    //aa = 5990;
    //std::cout << aa << std::endl;
  });

  BB->Run();
  std::cout << " game over ! " << std::endl;

  while(1) {
    // b.cs[0]->send("------------------------from main -----------");
    usleep(1000000);
  }
  usleep(8000000);


}

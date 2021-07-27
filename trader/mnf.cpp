#include "uBEE.h"
#include "Indicator.h"
#include <thread>
#include <unistd.h>
#include <iostream>
#include <vector>
#include <time.h>
#include <sys/time.h>

using namespace uBEE;
int main()
{

  struct  timeval start;
  struct  timeval end;
  unsigned  long diff;


  //------------------ 定义合约 及操作周期 --------------------------
  //31  32   33   34   35
  //const int fr[5] = {19,14401,9900,350,6600};

  std::map< std::string, std::vector<int> > fuMap ;
  fuMap["xu1801"] = {0,60,300,3600};
  fuMap["tu1801"] = {0,60,300,3600};
  fuMap["ru1805"] = {0,5,60,300,3600};
  fuMap["ru1809"] = {5,60};
  fuMap["xu1807"] = {60,19,300,3600};
  fuMap["zz1805"] = {5,15,30,60,300,3600,14401};
  fuMap["ag1912"] = {0,5,15,30,60,300,3600,14401};

  //-------------------- 变量定义 -----------------------------------
  uBEE::Base *BB = new uBEE::Base();
  //BB->Mode = 4;
  BB->Mode = 3;
  BB->FuInit(&fuMap);
  //-------------------- 变量定义 -----------------------------------

  sRSI *r;

  sEKE *e1;
  sEKE *e2;
  sEKE *e3;
  sEKE *e4;

  sMNF *m1;
  sMNF *m2;
  sMNF *m3;
  sMNF *m4;

  //-------------------- initialize -----------------------------------
  //getFutureBars(BB,(const char*)"ru1805",(const char *)"20170101",(const char *) "20180101",600);

  std::string ID = "ag1912";
  std::string st = "20170101";
  std::string en = "20190501";
  int  frequency = 300;
  uBEE::getFutureBars(BB,ID,st,en,frequency);

  gettimeofday(&start,NULL);
  gettimeofday(&end,NULL);
  diff = 1000000 * (end.tv_sec-start.tv_sec)+ end.tv_usec-start.tv_usec;
  diff = diff/10000;
  printf("thedifference is %ld\n",diff);


  int aa = 1009;

  BB->onDaily([]() {

  });

  BB->onWeekly([]() {

  });

  BB->onMonthly([]() {

  });

  //-------------------- onInit -----------------------------------
  BB->onInit([&]() {
    std::cout << "BB->onInit() \n" ;
    r = new sRSI(BB,(char*)"ag1912", 5, 14);
    std::cout << "uuuuuuuuuuadsfafd\n" ;
    e1 = new sEKE(BB,(char*)"ag1912",5, 36, 1, 12, 4);
    e2 = new sEKE(BB,(char*)"ag1912",5, 9*16, 8, 3*16, 16);
    e3 = new sEKE(BB,(char*)"ag1912",5, 9*64, 21, 3*64, 64);
    e4 = new sEKE(BB,(char*)"ag1912",5, 9*64*4, 34, 3*64*4, 64*4);
    m1 = new sMNF(BB,e1,(char*)"ag1912",5, 36);
    m2 = new sMNF(BB,e2,(char*)"ag1912",5, 9*16);
    m3 = new sMNF(BB,e3,(char*)"ag1912",5, 9*64);
    m4 = new sMNF(BB,e4,(char*)"ag1912",5, 9*64*4);
    std::cout << "onInit end !! whyyyyy333333\n" ;
  });
  //-------------------- onTick -----------------------------------
  BB->onTick([&](sTick *tick) {
    std::cout << "tick mnf4::" <<  m4->MNF[m4->x] << std::endl;
    std::cout << tick->LastPrice << std::endl;
  });

  //-------------------- onBars -----------------------------------
  BB->onBars([&](sKbar * bar[], int len) {
    for(int i=0; i<len; ++i) {
      // --------------------- F1 begin ---------------------------------
      if(bar[i]->iF == 5) {
        std::cout << r->RSI[r->x] << std::endl;
        std::cout << "bar mnf4::" <<  m4->MNF[m4->x] << std::endl;
      } // ----- end --- F1 60-----------

    } // -----------------for -----
  });

//-------------------- Run!! -----------------------------------
  BB->Run();
  std::cout << " game over ! " << std::endl;

  while(1) {
    usleep(1000000);
  }
}

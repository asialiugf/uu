#include "uBEE.h"
#include "Indicator.h"
#include <thread>
#include <unistd.h>
#include <iostream>
#include <vector>

using namespace uBEE;
int main()
{

  struct  timeval start;
  struct  timeval end;


  //------------------ 定义合约 及操作周期 --------------------------
  //31  32   33   34   35
  //const int fr[5] = {19,14401,9900,350,6600};

  std::map< std::string, std::vector<int> > fuMap ;
  fuMap["xu1801"] = {0,60,300,3600};
  fuMap["ru1805"] = {5,60,300,3600};
  fuMap["ru1809"] = {5,60};
  fuMap["xu1807"] = {60,19,300,3600};
  fuMap["zz1805"] = {5,15,30,60,300,3600,14401};
  fuMap["ag1809"] = {5,15,30,60,300,3600,14401};

  //-------------------- 变量定义 -----------------------------------
  uBEE::Base *BB = new uBEE::Base();
  BB->Mode = 4;
  BB->FuInit(&fuMap);
  int idx5 = BB->GetFrequencyIdx(5);
  int idx60 = BB->GetFrequencyIdx(60);
  int idx300 = BB->GetFrequencyIdx(300);
  idx300 = GetFrequencyIdx(300);

  D_OHLC S5 ;
  D_OHLC *F1 = new D_OHLC() ;
  D_OHLC F5 ;
  //int x;
  //-------------------- 变量定义 -----------------------------------
  D_KDJ *S5_1 = new D_KDJ(&S5);
  D_KDJ *S5_2 = new D_KDJ(&S5);
  D_KDJ *S5_3 = new D_KDJ(&S5);
  D_KDJ *S5_4 = new D_KDJ(&S5);

  D_KDJ *F1_1 = new D_KDJ(F1);
  D_KDJ *F1_2 = new D_KDJ(F1);
  D_KDJ *F1_3 = new D_KDJ(F1);
  D_KDJ *F1_4 = new D_KDJ(F1);

  D_RSI *rsi = new D_RSI(F1);

  D_STC *T1 = new D_STC(F1_1);
  D_STC *T2 = new D_STC(F1_2);
  D_STC *T3 = new D_STC(F1_3);
  D_STC *T4 = new D_STC(F1_4);

  G_KDJ *G = new G_KDJ(F1_1,F1_2,F1_3,F1_4);
  //-------------------- 变量定义 -----------------------------------
  //exit(0);


  //-------------------- initialize -----------------------------------
  gettimeofday(&start,NULL);
  gettimeofday(&end,NULL);
  unsigned  long diff = 1000000 * (end.tv_sec-start.tv_sec)+ end.tv_usec-start.tv_usec;
  diff = diff/10000;
  printf("thedifference is %ld\n",diff);


  int aa = 1009;
  BB->onInit([&]() {


  });
  //-------------------- onTick -----------------------------------
  BB->onTick([&aa,&BB](sTick *tick) {
    //std::cout << BB->fu->ID2 <<" "<<BB->fu->mMPF<<" "<< BB->fu->mLot<<" "<<BB->fu->mOP<<" "<<BB->fu->mCP<< std::endl;
    char f[512];
    snprintf(ca_errmsg,ERR_MSG_LEN,"A:%s %s %06d S:%d T:%s H:%g L:%g LP:%g AP:%g AV:%d BP:%g BV:%d OI:%g V:%d",
             tick->ActionDay,   tick->UpdateTime,
             tick->UpdateMillisec*1000, 0,            tick->TradingDay,
             tick->HighestPrice, tick->LowestPrice,   tick->LastPrice,
             tick->AskPrice1,    tick->AskVolume1,
             tick->BidPrice1,    tick->BidVolume1,
             tick->OpenInterest, tick->Volume);
    //std::cout << ca_errmsg <<std::endl;
    snprintf(f,512,"../exe/data/tick/%s.%s.tick.txt",tick->InstrumentID,tick->ActionDay);
    SaveLine(f,ca_errmsg) ;
  });

  //-------------------- onBars -----------------------------------
  int x = 0;   //非常重要的计数器
  int S5x = 0;
  int F1x = 0;
  BB->onBars([&](sKbar * bar[], int len) {
    //std::cout << BB->fu->ID2 <<" "<<BB->fu->mMPF<<" "<< BB->fu->mLot<<" "<<BB->fu->mOP<<" "<<BB->fu->mCP<< std::endl;
    char f[512];
    for(int i=0; i<len; ++i) {
      snprintf(ca_errmsg,ERR_MSG_LEN,"%s A:%s T:%s %s--%s O:%g H:%g L:%g C:%g V:%d vsam:%d",
               BB->InstrumentID, BB->ActionDay, BB->TradingDay,
               bar[i]->cB, bar[i]->cE,
               bar[i]->o, bar[i]->h, bar[i]->l, bar[i]->c,
               bar[i]->v, bar[i]->vsum) ;
      std::cout << ca_errmsg << std::endl;
      snprintf(f,512,"../exe/data/%s_%d_%di",BB->InstrumentID, bar[i]->iF, bar[i]->iX);
      SaveLine(f,ca_errmsg) ;


      // --------------------- F1 begin ---------------------------------
      if(bar[i]->iF == 60) {
        F1->Insert(bar[i]);                 // add OHLC
        F1_1->Update(36, 12, 12, 4);     // calculate kdj
        F1_2->Update(9*16, 3*16, 3*16, 16);     // calculate kdj
        F1_3->Update(9*64, 3*64, 3*64, 64);     // calculate kdj
        F1_4->Update(9*64*4, 3*64*4, 3*64*4, 64*4);     // calculate kdj
        rsi->Update(14) ;

        T1->Update(36) ;
        T2->Update(9*16) ;
        T3->Update(9*64) ;
        T4->Update(9*64*4) ;

        G->Update();
        //---------for test ---------------------
        std::cout <<"ssssssss:"<<rsi->RSI[rsi->x] << std::endl;
        std::cout <<"TTTTTT1 2 3 4:"<<T1->STC[T1->x]<<" "<<T2->STC[T2->x]<<" "<<T3->STC[T3->x]<<" "<<T4->STC[T4->x] <<" "<< std::endl ;
        std::cout <<"kkkkkk: "<<BB->ActionDay<<" "<<bar[i]->cB<<"-"<<bar[i]->cE<<" "<<F1_1->K[F1->x]<<" "<<F1_2->K[F1->x]<<" ";
        std::cout <<F1_3->K[F1->x]<<" "<<F1_4->K[F1->x] <<std::endl;
        std::cout <<"cccccc: "<<BB->ActionDay<<" "<<bar[i]->cB<<"-"<<bar[i]->cE<<" "<<F1->O[F1->x]<<" "<<F1->H[F1->x] <<" ";
        std::cout <<F1->L[F1->x]<<" "<<F1->C[F1->x] <<std::endl;

        std::cout <<"ggggggggg:"<<BB->ActionDay<<" "<<" 2Kc:"<<G->KDJ2->Kc<<" ku1:"<<G->ku1<<" kd1:";
        std::cout <<G->kd1 <<" "<<G->Ku1[G->ku1]<<" "<<G->Kd1[G->kd1]<<std::endl;
        std::cout <<"ggggggggg:"<<BB->ActionDay<<" "<<" 3Kc:"<<G->KDJ3->Kc<<" ku2:"<<G->ku2<<" kd2:";
        std::cout <<G->kd2 <<" "<<G->Ku2[G->ku2]<<" "<<G->Kd2[G->kd2]<<std::endl;
        std::cout <<"ggggggggg:"<<BB->ActionDay<<" "<<" 4Kc:"<<G->KDJ4->Kc<<" ku3:"<<G->ku3<<" kd3:";
        std::cout <<G->kd3 <<" "<<G->Ku2[G->ku3]<<" "<<G->Kd3[G->kd3]<<std::endl;
        std::cout <<"ggggggggg:"<<BB->ActionDay<<" "<<" 2Ec:"<<G->KDJ2->Ec<<" eu1:"<<G->eu1<<" ed1:";
        std::cout <<G->ed1 <<" "<<G->Eu1[G->eu1]<<" "<<G->Ed1[G->ed1]<<std::endl;
        std::cout <<"ggggggggg:"<<BB->ActionDay<<" "<<" 3Ec:"<<G->KDJ3->Ec<<" eu2:"<<G->eu2<<" ed2:";
        std::cout <<G->ed2 <<" "<<G->Eu2[G->eu2]<<" "<<G->Ed2[G->ed2]<<std::endl;
        std::cout <<"ggggggggg:"<<BB->ActionDay<<" "<<" 4Ec:"<<G->KDJ4->Ec<<" eu3:"<<G->eu3<<" ed3:";
        std::cout<<G->ed3 <<" "<<G->Eu3[G->eu3]<<" "<<G->Ed3[G->ed3]<<std::endl;
        //---------for test ---------------------


        if(F1->x > 700) {
          BB->fu->DStopLost(1,bar[i]->c);
          BB->fu->StopProfit(1,bar[i]->c);
          BB->fu->CurrPL(bar[i]->c) ;
          std::cout << "current pl:" << BB->fu->cPL << std::endl;

          /*
          if(F1_1->Kx>0 && F1_2->Kx>0 && F1_3->Kx>0) {
            if(BB->fu->NL==0) {
              BB->fu->SellShort(1,bar[i]->c);
              BB->fu->BuyLong(1,bar[i]->c);
              std::cout <<"llllll:"<<BB->ActionDay<<" "<<bar[i]->cB<<"-"<<bar[i]->cE;
              std::cout <<" mPL:"<<BB->fu->mPL<< " NL:"<< BB->fu->NL<< " NS:"<< BB->fu->NS << std::endl;
            }
          } else if(F1_1->Kx<0 && F1_2->Kx<0 && F1_3->Kx<0) {
            if(BB->fu->NS==0) {
              BB->fu->SellLong(1,bar[i]->c);
              BB->fu->BuyShort(1,bar[i]->c);
              std::cout <<"ssssss:"<<BB->ActionDay<<" "<<bar[i]->cB<<"-"<<bar[i]->cE;
              std::cout <<" mPL:"<<BB->fu->mPL<< " NL:"<< BB->fu->NL<< " NS:"<< BB->fu->NS << std::endl;
            }
          }
          */
          if(G->KDJ3->Ec >=1) {
            if(F1_1->Kx>0 && F1_2->Kx>0) {
              BB->fu->SellShort(1,bar[i]->c);
              BB->fu->BuyLong(1,bar[i]->c);
              std::cout <<"llllll:"<<BB->ActionDay<<" "<<bar[i]->cB<<"-"<<bar[i]->cE;
              std::cout <<" mPL:"<<BB->fu->mPL<< " NL:"<< BB->fu->NL<< " NS:"<< BB->fu->NS << std::endl;
            }
          }
          if(G->KDJ3->Ec <=-1) {
            //  if(G->ed1==1) {
            if(F1_1->Kx<0 && F1_2->Kx<0) {
              BB->fu->SellLong(1,bar[i]->c);
              BB->fu->BuyShort(1,bar[i]->c);
              std::cout <<"ssssss:"<<BB->ActionDay<<" "<<bar[i]->cB<<"-"<<bar[i]->cE;
              std::cout <<" mPL:"<<BB->fu->mPL<< " NL:"<< BB->fu->NL<< " NS:"<< BB->fu->NS << std::endl;
            }
          }
        }

      } // ----- end --- F1 -----------



    } // -----------------for -----
  });

//-------------------- Run!! -----------------------------------
  BB->Run();
  std::cout << " game over ! " << std::endl;

  while(1) {
    usleep(1000000);
  }
}

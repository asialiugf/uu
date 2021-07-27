#include "../ctp/ThostFtdcMdApi.h"
#include <uWS/uWS.h>
#include "Bars.h"
#include "MkSim.h"
#include "Global.h"
#include "ErrLog.h"
#include "File.h"
#include <map>
#include <string>
#include <iostream>
#include <cstdio>
#include <ctime>
#include <vector>

namespace uBEE
{
#define LOG_FUNC()    do { cout << __FUNCTION__ << endl; } while (0)
//---
uWS::Group<uWS::SERVER> * SimSG;
std::map<std::string,uBEE::FuSim*>    M_FuSim;         // 每个期货一个 FuSim  构成一个MAP
std::map<std::string,uBEE::FuBo*>     M_SimFuBo;         // 每个期货一个 FuBlock，构成一个MAP
std::vector<std::string>             V_Future(10);
std::map<std::string,std::string>    M_SimFuFile;         // 每个期货一个 FuBlock，构成一个MAP
//barSG  * KBuf;

//---

// --- FuSim 每个期货一个FuSim,用于处理 模拟交易产生相应的tick ... bar 等.
FuSim::FuSim(uBEE::FuBo *fu, char *Future, const char *pFile)
{
  see_memzero(File,1024);
  see_memzero(Table,512);
  see_memzero(InstrumentID,31);
  snprintf(InstrumentID,31,"%s",Future) ;
  snprintf(Tick.InstrumentID,31,"%s",Future) ;  // InstrumentID 是成员变量
  snprintf(File,1024,"%s",pFile) ;
  fubo = fu ;

  iLineNum = CountLines(pFile) ;
  iCurLine = 1;
  if(Future !=nullptr && strlen(Future) <= 31) {
    memcpy(InstrumentID,Future,strlen(Future));
  }
  nTick = &aTick;
  nTick->iType = T_TICK ;
  nData = &aData;
  nData->iType = T_BARS ;
}

int FuSim::SetBarF(const char *pFile)
{
  see_memzero(BarF,1024);
  snprintf(BarF,1024,"%s",pFile) ;
  return 0;
}
//---- 一次运行一个文件 --------------------------------------------------
int FuSim::RunTickF()
{
  int ss;
  //sTick * nTick = &stTick;  //
  //nTick->iType = T_TICK;    // 在构造函数中初始化！
  string temp;
  fstream file;
  file.open(File,ios::in);
  while(getline(file,temp)) {

    //snprintf(nTick->InstrumentID,31,"%s",Future) ;  // 在构造函数中初始化！
    sscanf(temp.c_str(), "A:%s %s %d S:%d T:%s H:%lf L:%lf LP:%lf AP:%lf AV:%d BP:%lf BV:%d OI:%lf V:%d",
           nTick->ActionDay,
           nTick->UpdateTime,
           &nTick->UpdateMillisec, &ss,
           nTick->TradingDay,
           &nTick->HighestPrice,
           &nTick->LowestPrice,
           &nTick->LastPrice,
           &nTick->AskPrice1, &nTick->AskVolume1,
           &nTick->BidPrice1, &nTick->BidVolume1,
           &nTick->OpenInterest, &nTick->Volume);
    nTick->UpdateMillisec = nTick->UpdateMillisec/1000;

    fubo->SG->broadcast((const char*)nTick, tLen, uWS::OpCode::BINARY);
  }
  file.close();
  return 0;

}

//---- 每次读一行 ----------
TICK * FuSim::MkTickF()             // make tick from tick file
{
  int ss;
  if(iLineNum<=0) {
    return nullptr ;
  }
  if(iCurLine <= iLineNum) {
    std::string TickLine =  ReadLine(File,iCurLine,iLineNum) ;
    std::cout << " iCurLine:" << iCurLine << " iLineNum:" << iLineNum <<" " << TickLine << std::endl;
    if(TickLine.empty()) {
      return nullptr ;
    }

    //snprintf(Tick.InstrumentID,31,"%s",Future) ;  // 在构造函数中初始化！
    sscanf(TickLine.c_str(), "T:%s %s %d S:%d A:%s H:%lf L:%lf LP:%lf AP:%lf AV:%d BP:%lf BV:%d OI:%lf V:%d",
           Tick.TradingDay,
           Tick.UpdateTime,
           &Tick.UpdateMillisec, &ss,
           Tick.ActionDay,
           &Tick.HighestPrice,
           &Tick.LowestPrice,
           &Tick.LastPrice,
           &Tick.AskPrice1, &Tick.AskVolume1,
           &Tick.BidPrice1, &Tick.BidVolume1,
           &Tick.OpenInterest, &Tick.Volume);
    Tick.UpdateMillisec = Tick.UpdateMillisec/1000;

    std::cout << "MkTick:" << Tick.HighestPrice << " LP:" << Tick.LowestPrice << " LastPrice:" << Tick.LastPrice << std::endl;

    iCurLine++ ;
  }
  if(iCurLine>iLineNum) {
    exit(0);
    iCurLine = 1;
  }
  return &Tick ;
}


int FuSim::RunBarsF(int Fr)       // make bars from bars file
{
  char        cBE[30];   // "xx:xx:xx--xx:xx:xx"
  //sData * nData = &Data;
  //nData->iType = T_BARS ;

  string temp;
  fstream file;
  // charmi  BarF 以后可以放在 fubo中。 future block !!
  file.open(BarF,ios::in);
  while(getline(file,temp)) {
    //sscanf(temp.c_str(), "T:%s %s %d S:%d A:%s H:%lf L:%lf LP:%lf AP:%lf AV:%d BP:%lf BV:%d OI:%lf V:%d",
    //       ru1809 A:20180330 T:20180330 13:51:00--13:51:59 O:11585 H:11590 L:11575 C:11575 V:230950 vsam:230950
    sscanf(temp.c_str(), "%s A:%s T:%s %s O:%lf H:%lf L:%lf C:%lf V:%d vsam:%d",
           nData->InstrumentID, nData->ActionDay, nData->TradingDay, cBE,
           &nData->KK[0].o,
           &nData->KK[0].h,
           &nData->KK[0].l,
           &nData->KK[0].c,
           &nData->KK[0].v,
           &nData->KK[0].vsum);

    nData->KK[0].iX = 9; // need to be modified !!!
    nData->KK[0].iF = Fr ;
    memcpy(nData->KK[0].cB,cBE,8);
    memcpy(nData->KK[0].cE,cBE+10,8);
    nData->KK[0].cB[8] = '\0';
    nData->KK[0].cE[8] = '\0';

    nData->iN = 1;
    //fubo 是成员变量
    fubo->SG->broadcast((const char*)nData, oLen, uWS::OpCode::BINARY);
    std::cout<< nData->KK[0].cB <<"--"<<nData->KK[0].cE<<" O:"<< nData->KK[0].o << " C:"<< nData->KK[0].c << std::endl;
  } //--while
  file.close();
}


//----------------------------------------------------------------------------------
int FuSim::RunTickBarsF()
{
  int ss;
  string temp;
  fstream file;
  TICK  TickTemp;
  file.open(File,ios::in);
  while(getline(file,temp)) {

    sscanf(temp.c_str(), "A:%s %s %d S:%d T:%s H:%lf L:%lf LP:%lf AP:%lf AV:%d BP:%lf BV:%d OI:%lf V:%d",
           Tick.ActionDay,
           Tick.UpdateTime,
           &Tick.UpdateMillisec, &ss,
           Tick.TradingDay,
           &Tick.HighestPrice,
           &Tick.LowestPrice,
           &Tick.LastPrice,
           &Tick.AskPrice1, &Tick.AskVolume1,
           &Tick.BidPrice1, &Tick.BidVolume1,
           &Tick.OpenInterest, &Tick.Volume);
    Tick.UpdateMillisec = Tick.UpdateMillisec/1000;
    
    //int64_t lp1 = (int64_t)Tick.LastPrice * 1000000;
    //int64_t lp2 = (int64_t)TickTemp.LastPrice * 1000000;
    int64_t lp1 = (int64_t)(Tick.Volume) ;
    int64_t lp2 = (int64_t)(TickTemp.Volume);
    int64_t xordat =  lp1 ^ lp2;
    int8_t HeadZeros = __builtin_clzll(xordat);
    int8_t TailZeros = __builtin_ffsll(xordat);
    std::cout <<"Volume1:"<<Tick.Volume << "Volume2:"<<TickTemp.Volume << "lp1:"<< lp1 << " lp2:" << lp2 ;
    std::cout << " xordat:" << xordat <<  " HeadZeros: " << (int)HeadZeros << "  TailZeros: " << (int)TailZeros << std::endl;

    memcpy(&TickTemp,&Tick,sizeof(TICK));

    HandleTick(fubo,&Tick,SEND_ALL);
    usleep(50);

  }
  file.close();
  return 0;
}


int FuSim::MkTickBarsF(int Fr)   // make bars from tick file
{
  if(iLineNum ==0) {
    return -1;
  }

  return 0;
}

int FuSim::MkTickT()             // make tick from database tick table
{
}
int FuSim::MkBarsT(int Fr)       // make bars from database bars table
{
}
int FuSim::MkTickBarsT(int Fr)   // make bars from database tick table
{
}

void MkSim(uWS::Group<uWS::SERVER> * new_sg)
{

  if(T________) {
    uBEE::ErrLog(1000,"MkSim(): enter!!",1,0,0) ;
  }

  SimSG = new_sg;

  // --------- tb in Global.h ------------------------------------
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



  std::cout << " M_SimFuFile insert file " << std::endl;
  //M_SimFuFile.insert(std::pair<std::string,std::string>("ag1606","../Sim/tick/ag1606.tick.ss"));
  //M_SimFuFile.insert(std::pair<std::string,std::string>("ru1805","../Sim/tick/ru1805.20180330.tick.txt"));
  //M_SimFuFile.insert(std::pair<std::string,std::string>("zn1805","../Sim/tick/zn1805.20180328.tick.txt"));
  //M_SimFuFile.insert(std::pair<std::string,std::string>("ru1809","../Sim/tick/ru1809.20180404.tick.txt"));
  //M_SimFuFile.insert(std::pair<std::string,std::string>("ru1809","../Sim/tick/ru1809.20180326.tick.txt"));
  M_SimFuFile.insert(std::pair<std::string,std::string>("ru1805","../Sim/tick/ru1805.20180330.tick.txt"));
  // M_SimFuFile.insert(std::pair<std::string,std::string>("ag1808","../Sim/tick/ag1808.20180327.tick.txt"));
  //M_SimFuFile.insert(std::pair<std::string,std::string>("bu1606","../Sim/tick/bu1606.tick.ss"));
  //M_SimFuFile.insert(std::pair<std::string,std::string>("cu1603","../Sim/tick/cu1603.tick.ss"));
  //M_SimFuFile.insert(std::pair<std::string,std::string>("m1605","../Sim/tick/m1605.tick.ss"));
  //M_SimFuFile.insert(std::pair<std::string,std::string>("MA605","../Sim/tick/MA605.tick.ss"));

  // ------------------------------初始化 FuSim ， 每个 future  一个 FuSim ... 保存在  M_FuSim 这个map中。
  for(auto iter = M_SimFuFile.begin(); iter != M_SimFuFile.end(); ++iter) {

    char* p = (char*)iter->first.c_str();  // char *Future, const char *pFile
    char* f = (char*)iter->second.c_str();

    sprintf(ca_errmsg,"MkSim(): Future:%s file:%s",p,f) ;
    uBEE::ErrLog(1000,ca_errmsg,1,0,0) ;

    std::cout << "MkSim(): new FuBo Future Block :"<< p << std::endl;
    uBEE::FuBo *fubo = new uBEE::FuBo(p,tb,SimSG);
    M_SimFuBo.insert(std::pair<std::string,uBEE::FuBo *>(p, fubo));

    std::cout << "MkSim(): new FuSim Future Simulation :"<< p << std::endl;
    // --------- will call fusim->MkTick() -------------
    uBEE::FuSim *fusim = new uBEE::FuSim(fubo, p, f);
    // -------临时加 begin -----
    if(memcmp(p,"ru1809",6) ==0) {
      //fusim->SetBarF("../Sim/bars/ru1809_00_01_00.60.9i");
      fusim->SetBarF("../Sim/bars/ru1809_00_01_00.60.9i"); //设置要处理的OHLC文件.
    }
    // ------ 临时加 end -----
    M_FuSim.insert(std::pair<std::string,uBEE::FuSim*>(p, fusim));
  }

  /*
  // ----------------  test ----------------------------------------------begin
  for(auto it = M_SimFuBo.begin(); it != M_SimFuBo.end(); ++it) {
    uBEE::ErrLog(1000,"rrrrrrrrrr",1,0,0);
    uBEE::FuBo *fubo = it->second;

    for(int i=0; i<50; ++i) {
      if(fubo->pBaBo[i] == nullptr) {
        continue;
      }
      for(int j=0; j<fubo->pBaBo[i]->iSegNum; ++j) {
        Segment *seg = fubo->pBaBo[i]->seg[j] ;
        sprintf(ca_errmsg,"rrrrrrrrr:%s i:%d iF:%d id:%d:%d, mk:%d  sn:%d segBE:%s-%s barBE:%s--%s sgiBE:%d--%d  bariBE:%d--%d barBxEx:%d--%d",
                fubo->InstrumentID,
                i,fubo->pBaBo[i]->iF,j,fubo->pBaBo[i]->iSegNum,seg->mark,seg->sn,
                seg->cB,seg->cE,seg->barB,seg->barE,
                seg->iB,seg->iE,
                seg->bariB,seg->bariE,seg->barBx,seg->barEx);
        uBEE::ErrLog(1000,ca_errmsg,1,0,0);
      }
    }

    uBEE::ErrLog(1000,"rrrrrrrrrr",1,0,0);
  }
  // ----------------  test ----------------------------------------------end

  // -------------- 从 M_FuSim 这个map中取出 future 以及 fusim ，再 从 M_SimFuBo中 找出 这个future的 fobo--->
  // -------------- fusim 生成tick ,  fobo 用来生成 K柱 .

  usleep(10000000);

  //----------- bar test  from bar file ---------------------------------------

  for(auto it = M_FuSim.begin(); it != M_FuSim.end(); ++it) {
    uBEE::FuSim *fusim = it->second ;
    //std::map<std::string,uBEE::FuBo*>::iterator iter;
    auto iter = M_SimFuBo.find(it->first);
    if(iter == M_SimFuBo.end()) {
      sprintf(ca_errmsg,"MkSim not find: %s",it->first.c_str()) ;
      uBEE::ErrLog(1000,ca_errmsg,1,0,0) ;
      continue ;
    }
    uBEE::FuBo *fubo = iter->second;
    if(memcmp(it->first.c_str(),"ru1809",6)==0) {
      //std::cout <<"enter into send :fr:60:  "<< it->first << std::endl;
      //fusim->RunBarsF(60);
      ///
      struct  timeval start;
      struct  timeval end1;
      unsigned  long diff;
      gettimeofday(&start,NULL);
      fusim->RunTickBarsF();
      gettimeofday(&end1,NULL);
      diff = 1000000 * (end1.tv_sec-start.tv_sec)+ end1.tv_usec-start.tv_usec;
      printf("thedifference is %ld\n",diff);
      ///

    }
  }
  usleep(20000000);
  exit(0);

  */
  //----------- bar test  from tick file ---------------------------------------
  while(1) {
    for(auto it = M_FuSim.begin(); it != M_FuSim.end(); ++it) {
      //char * fu = it->first ;
      uBEE::FuSim *fusim = it->second ;
      // 从 M_SimFuBo 这个map中找到相应的 fubo， 然后将其传给 DealBars() ;
      std::map<std::string,uBEE::FuBo *>::iterator iter;
      iter = M_SimFuBo.find(it->first);
      if(iter == M_SimFuBo.end()) {
        sprintf(ca_errmsg,"MkSim not find: %s",it->first.c_str()) ;
        uBEE::ErrLog(1000,ca_errmsg,1,0,0) ;
        continue ;
      }

      uBEE::FuBo *fubo = iter->second;

      TICK *tick = fusim->MkTickF();

      memcpy(tick->InstrumentID,fubo->InstrumentID,strlen(fubo->InstrumentID)) ;

      if(fubo==nullptr || tick==nullptr) {
        continue;
      }

      std::cout << "tick:" << tick->HighestPrice << " LP:" << tick->LowestPrice << " LastPrice:" << tick->LastPrice << std::endl;

      HandleTick(fubo,tick,SEND_SAVE_ALL);
      //usleep(100000);
    }
  }

}

} // end namespace uBEE

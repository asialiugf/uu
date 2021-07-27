#include "../ctp/ThostFtdcMdApi.h"
#include <uWS/uWS.h>
#include "Bars.h"
#include "MkSim.h"
#include "Global.h"
#include "File.h"
#include <map>
#include <string>
#include <iostream>
#include <cstdio>
#include <ctime>
#include <vector>

namespace uBEE
{

//---
uWS::Group<uWS::SERVER> * SimSG;
std::map<std::string,uBEE::FuSim>    M_FuSim;         // 每个期货一个 FuSim  构成一个MAP
std::map<std::string,uBEE::FuBo>     M_SimFuBo;         // 每个期货一个 FuBlock，构成一个MAP
std::vector<std::string>             V_Future(10);
std::map<std::string,std::string>    M_SimFuFile;         // 每个期货一个 FuBlock，构成一个MAP

//---

// --- FuSim 每个期货一个FuSim,用于处理 模拟交易产生相应的tick ... bar 等.
FuSim::FuSim(char *Future, const char *pFile)
{
  see_memzero(File,1024);
  see_memzero(Table,512);
  see_memzero(InstrumentID,31);
  memcpy(File,pFile,strlen(pFile));

  iLineNum = CountLines(pFile) ;
  iCurLine = 0;
  if(Future !=nullptr && strlen(Future) <= 31) {
    memcpy(InstrumentID,Future,strlen(Future));
  }

}
TICK * FuSim::MkTickF()             // make tick from tick file
{
  return &Tick ;
}
int FuSim::MkBarsF(int Fr)       // make bars from bars file
{
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
    sprintf(ca_errmsg,"MkSim(): enter!!") ;
    uBEE::ErrLog(1000,ca_errmsg,1,0,0) ;
  }

  SimSG = new_sg;

  V_Future.push_back("ag1801");
  if(T________) {
    sprintf(ca_errmsg,"MkSim(): Future:%s",V_Future[0].c_str()) ;
    uBEE::ErrLog(1000,ca_errmsg,1,0,0) ;
  }

  for_each(V_Future.cbegin(), V_Future.cend(), [](string fu)->void{
    cout << fu << endl;
    char* p = (char*)fu.c_str();
    if(T________)
    {
      sprintf(ca_errmsg,"MkSim(): Future:%s",p) ;
      uBEE::ErrLog(1000,ca_errmsg,1,0,0) ;
    }

    // 用户自定义 交易周期
    int fr[5] = {19,14401,180,300,600};
    uBEE::FuBo *fubo = new uBEE::FuBo(p,tb,&fr[0], 5);
    M_SimFuBo.insert(std::pair<std::string,uBEE::FuBo>(fu, *fubo));

    uBEE::FuSim *fusim = new uBEE::FuSim(p, "ag1801.tick");
    M_FuSim.insert(std::pair<std::string,uBEE::FuSim>(p, *fusim));
  });

  while(1) {
    for(auto it = M_FuSim.begin(); it != M_FuSim.end(); ++it) {
      //char * fu = it->first ;
      uBEE::FuSim *fusim = &(it->second) ;
      // 从 M_SimFuBo 这个map中找到相应的 fubo， 然后将其传给 DearBars() ;
      std::map<std::string,uBEE::FuBo>::iterator iter;
      iter = M_SimFuBo.find(it->first);
      if(iter == M_SimFuBo.end()) {
        sprintf(ca_errmsg,"MkSim not find: %s",it->first.c_str()) ;
        uBEE::ErrLog(1000,ca_errmsg,1,0,0) ;
        continue ;
      }

      uBEE::FuBo *fubo = &(iter->second);

      TICK *tick = fusim->MkTickF();
      for(int i=0; i<50; i++) {
        DealBar(fubo, tick, i);
      }
    }
  }
}

} // end namespace uBEE

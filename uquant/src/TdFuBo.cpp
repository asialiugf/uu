#include "uBEE.h"
#include "TdFuBo.h"
#include "ErrLog.h"

namespace uBEE
{

sOHLC::sOHLC():
  O(100000,SEE_NULL),
  H(100000,SEE_NULL),
  L(100000,SEE_NULL),
  C(100000,SEE_NULL),
  V(100000,SEE_NULL)
{
  memset(cB,'\0',9);
  memset(cE,'\0',9);
  u = 0;  //表示当前的bar是已经结束了的bar。每个bar只有最后一个tick过来时，才会结束，中间的tick会update这个bar。
  x = -1;
}

/*
 x: index !
*/
int sOHLC::Insert(sKbar *bar)
{
  if(u==0) {
    x++;
  }
  O[x] = bar->o ;
  H[x] = bar->h ;
  L[x] = bar->l ;
  C[x] = bar->c ;
  V[x] = bar->v ;
  memcpy(cB,bar->cB,9);
  memcpy(cE,bar->cE,9);
  u = 0;
  return 0;
}

int sOHLC::Update(sKbar *bar)
{
  if(u==0) {
    x++;
    u = 1;
  }
  O[x] = bar->o ;
  H[x] = bar->h ;
  L[x] = bar->l ;
  C[x] = bar->c ;
  V[x] = bar->v ;
  memcpy(cB,bar->cB,9);
  memcpy(cE,bar->cE,9);
  return 0;
}


//=========================================================================
/*
 * input: futureID
 * 策略程序 初始化时， 为每个期货品种生成一个 sFuBo block, 记录 最小变动价格，手续费等初始化信息。
*/

sFuBo::sFuBo(std::string f)
{
  const char *id = f.c_str() ;

  see_memzero(InstrumentID,31);
  see_memzero(ID2,3);

  char fn[3] ;
  see_memzero(fn,3);
  memcpy(fn,id,2) ;
  if(fn[1] <= '9' && fn[1] >= '0') {
    fn[1] = '\0';
  }
  memcpy(ID2,fn,strlen(fn));
  memcpy(InstrumentID,id,strlen(id));

  //std::map<std::string,std::string>::const_iterator it;
  auto it = M_FuRate.find(ID2);
  if(it==M_FuRate.end()) {
    sprintf(ca_errmsg,"M_FuRate in TdFuBo.h : %s %s not found!",InstrumentID,ID2) ;
    uBEE::ErrLog(1000,ca_errmsg,1,0,0) ;
  } else {
    sscanf(it->second.c_str(), "mMPF:%lf mLot:%lf mOP:%lf mCP:%lf",
           &mMPF,   &mLot,   &mOP,   &mCP);
  }

  for(int i=0; i<50; i++) {
    iP[i] = -1;
  }
  mPL = 0;
  cPL = 0;
  NL = 0;
  NS = 0;
} // ---------


// n 下单手数 c:信号产生时的收盘价
int sFuBo::BuyShort(int n, double c)
{
  NS = n;
  SP = c - mMPF ;
  std::cout << "TdFuBo:BuyShort:SP:"<<SP<< std::endl;
  return 0;
}
int sFuBo::BuyLong(int n, double c)
{
  NL = n;
  LP = c + mMPF ;
  std::cout << "TdFuBo:BuyLong:LP:"<<LP<< std::endl;
  return 0;
}
int sFuBo::SellShort(int n, double c)
{
  if(NS < n) {
    return -1;
  }
  NS = 0;
  mPL += (SP-c-mMPF)/mMPF*mLot*n ;
  std::cout << "TdFuBo:SellShort:mPL:"<<mPL<< std::endl;
  return 0;
}
int sFuBo::SellLong(int n, double c)
{
  if(NL < n) {
    return -1;
  }
  NL = 0;
  mPL += (c-mMPF-LP)/mMPF*mLot*n ;
  std::cout << "TdFuBo:SellLong:mPL:"<<mPL<< std::endl;
  return 0;
}
// ------- 动态止损 ----------------------------
int sFuBo::DStopLost(int n, double c)
{
  if(NS >= n) {
    if((SP-c-mMPF)/mMPF <= -15) {
      NS = 0;
      mPL += (SP-c-mMPF)/mMPF*mLot*n ;
      std::cout << "TdFuBo:DStopLost:SP:"<<SP<<" c:"<<c<<" "<< (SP-c-mMPF)/mMPF << std::endl;
      return 0;
    }
    if(SP>c) {
      mPL += (SP-c-mMPF)/mMPF*mLot*n ;
      SP = c;
    } else if(SP<c) {
      //mPL += (SP-c-mMPF)/mMPF*mLot*n ;
    }
  }

  if(NL >= n) {
    if((c-mMPF-LP)/mMPF <= -15) {
      NL = 0;
      mPL += (c-mMPF-LP)/mMPF*mLot*n ;
      std::cout << "TdFuBo:DStopLost:LP:"<<LP<<" c:"<<c<<" "<< (c-mMPF-LP)/mMPF << std::endl;
      return 0;
    }
    if(LP<c) {
      mPL += (c-mMPF-LP)/mMPF*mLot*n ;
      LP = c;
    } else if(LP>c) {
      //todo
    }
  }
}

// ------- 止损 ----------------------------
int sFuBo::StopLost(int n, double c)
{
  if(NS >= n) {
    if((SP-c-mMPF)/mMPF <= -8) {
      NS = 0;
      mPL += (SP-c-mMPF)/mMPF*mLot*n ;
      std::cout << "stoplost:SP:"<<SP<<" c:"<<c<<" "<< (SP-c-mMPF)/mMPF << std::endl;
      return 0;
    }
  }

  if(NL >= n) {
    if((c-mMPF-LP)/mMPF <= -8) {
      NL = 0;
      mPL += (c-mMPF-LP)/mMPF*mLot*n ;
      std::cout << "stoplost:LP:"<<LP<<" c:"<<c<<" "<< (c-mMPF-LP)/mMPF << std::endl;
      return 0;
    }
  }
}

//--------- 止赢 ----------------------------
int sFuBo::StopProfit(int n, double c)
{
  if(NS >= n) {
    if((SP-c-mMPF)/mMPF >= 4) {
      NS = 0;
      mPL += (SP-c-mMPF)/mMPF*mLot*n ;
      std::cout << "TdFuBo:stopprofit:SP:"<<SP<<" c:"<<c<<" "<< (SP-c-mMPF)/mMPF << std::endl;
      return 0;
    }
  }

  if(NL >= n) {
    if((c-mMPF-LP)/mMPF >= 4) {
      NL = 0;
      mPL += (c-mMPF-LP)/mMPF*mLot*n ;
      std::cout << "TdFuBo:stopprofit:LP:"<<LP<<" c:"<<c<<" "<< (c-mMPF-LP)/mMPF << std::endl;
      return 0;
    }
  }
  return 0;
}

// ----------动态计算显示当前收益 -----------------
int sFuBo::CurrPL(double c)
{
  if(NS > 0) {
    cPL = mPL + (SP-c-mMPF)/mMPF*mLot*NS ;
    return 0;
  }
  if(NL >0) {
    cPL = mPL + (c-mMPF-LP)/mMPF*mLot*NL ;
    return 0;
  }
} // --------

} //end namespace

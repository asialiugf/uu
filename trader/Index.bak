#include "Index.h"
#include "uBEE.h"
#include <stdlib.h>
#include <vector>

namespace uBEE
{
using namespace std;
using namespace uBEE;

D_OHLC::D_OHLC():
  O(100000,SEE_NULL),
  H(100000,SEE_NULL),
  L(100000,SEE_NULL),
  C(100000,SEE_NULL),
  V(100000,SEE_NULL)
{
  memset(cB,'\0',9);
  memset(cE,'\0',9);
  x = -1;
}
/*
 x: index !
*/
int D_OHLC::Insert(sKbar *bar)
{
  x++;
  O[x] = bar->o ;
  H[x] = bar->h ;
  L[x] = bar->l ;
  C[x] = bar->c ;
  V[x] = bar->v ;
  memcpy(cB,bar->cB,9);
  memcpy(cE,bar->cE,9);
  return 0;
}

//--------------- uRSI -----------------------------------------------------
uRSI::uRSI(uBEE::Base *BB,char *ID, int Frequency, int n):
  _max(100000,SEE_NULL),
  _abs(100000,SEE_NULL),
  _sma1(100000,SEE_NULL),
  _sma2(100000,SEE_NULL),
  RSI(100000,SEE_NULL)
{
  Future *fu ;
  int    Fidx ;
  int    iC ;
  map<std::string,uBEE::Future>::iterator it;

  it = BB->M_Fu.find(ID);
  if(it != BB->M_Fu.end()) {
    fu = &it->second ;
  }
  Fidx = GetFrequencyIdx(Frequency);
  ohlc = fu->pBars[Fidx];
  N = n;
  x = -1;
  ls = 0;


  iC = fu->pBars[Fidx]->iCall ;
  /*
  for(int i = 0; i < iC; i++) {
    if(fu->callbacks[i] == Call) {
      break;
    }
  }
  */
  //typedef void* (*FUNC)();//定义FUNC类型是一个指向函数的指针，该函数参数为void*，返回值为void*
  //FUNC callback = (FUNC)&uRSI::Call;//强制转换func()的类型
  if(iC<256) {
    fu->pBars[Fidx]->callbacks[iC] = Call;
  }
  fu->pBars[Fidx]->iCall++;

}

/*
static void uRSI::CB()
{
  Call();
}
*/

void Call()
{
  x = ohlc->x ;
  SEE_RSI(x,x,&ohlc->C[0],N,&_max[0],&_abs[0],&_sma1[0],&_sma2[0],&RSI[0]);
  if(x <=2) {
    return ;
  }
  if(RSI[x] >30 && RSI[x-1]<30) {
    ls = 1;
  } else if(RSI[x] <70 && RSI[x-1]>70) {
    ls = -1;
  } else {
    ls = 0;
  }
  return ;
}

//--------------- RSI -----------
D_RSI::D_RSI(D_OHLC * o):
  _max(100000,SEE_NULL),
  _abs(100000,SEE_NULL),
  _sma1(100000,SEE_NULL),
  _sma2(100000,SEE_NULL),
  RSI(100000,SEE_NULL)
{
  ohlc = o;
  x = -1;
  ls = 0;
}

int D_RSI::Update(int N)
{
  x = ohlc->x ;
  SEE_RSI(x,x,&ohlc->C[0],N,&_max[0],&_abs[0],&_sma1[0],&_sma2[0],&RSI[0]);
  if(x <=2) {
    return 0 ;
  }
  if(RSI[x] >30 && RSI[x-1]<30) {
    ls = 1;
  } else if(RSI[x] <70 && RSI[x-1]>70) {
    ls = -1;
  } else {
    ls = 0;
  }
  return 0;
}
//--------------- RSI end -----------
//--------------- STC begin -----------
D_STC::D_STC(D_KDJ *k):
  STC(100000,SEE_NULL)
{
  preH = SEE_NULL;
  preL = SEE_NULL;
  preF = SEE_NULL;
  kdj = k;
  x = -1;
}

int D_STC::Update(int N)
{
  x = kdj->x ;
  SEE_RSV(x, x, &kdj->K[0], &kdj->K[0], &kdj->K[0], &preH, &preL, &preF, N, &STC[0]);
  return 0;
}
//--------------- STC end -----------

//--------------- D_MNF begin -------------
D_MNF::D_MNF(D_EKE *e):
  MNF(100000,SEE_NULL)
{
  preH = SEE_NULL;
  preL = SEE_NULL;
  preF = SEE_NULL;
  KE = e;
  x = -1;

  ud = -1;
  udN = 1;

  uN = 0;
  dN = 0;

  ls = 0 ;
  lsN = 0 ;
}

int D_MNF::Update(int N)
{
  x = KE->x ;            // kdj
  SEE_RSV(x, x, &KE->K[0], &KE->K[0], &KE->K[0], &preH, &preL, &preF, N, &MNF[0]);

  if(x<=2) {
    return 0;
  }

  // --------- calculate ud -----------
  if(MNF[x] >= 100) {
    if(MNF[x-1] < 100) {
      ud = 100 ;
      udN = 1;
    } else {
      udN +=1 ;
    }
  } else if(MNF[x] <= 0) {
    if(MNF[x-1] > 0) {
      ud = 0;
      udN = 1;
    } {
      udN +=1 ;
    }
  } else {
    if(MNF[x-1] >= 100) {
      ud = 99;
      udN = 1;
    } else if(MNF[x-1] <= 0) {
      ud = 1;
      udN = 1;
    } else {
      udN +=1 ;
    }
  }

  // --------- calculate ls -----------
  if(MNF[x] > MNF[x-1]) {
    if(MNF[x-1] < MNF[x-2]) {
      ls = 1;
      lsN = 1;
    } else {
      ls = 2;
      lsN +=1;
    }
  } else if(MNF[x] < MNF[x-1]) {
    if(MNF[x-1] > MNF[x-2]) {
      ls = -1;
      lsN = 1;
    } else {
      ls = -2;
      lsN +=1;
    }
  } else {
    lsN +=1;
  }
  //-------------------------------------

  return 0;
}
//--------------- D_MNF end  -------------
//--------D_EKE begin ------------------------------------------
D_EKE::D_EKE(D_OHLC * o):
  _R(100000,SEE_NULL),
  _E(100000,SEE_NULL),
  K(100000,SEE_NULL),
  E(100000,SEE_NULL)
{
  preH = SEE_NULL;
  preL = SEE_NULL;
  preF = SEE_NULL;
  x = -1;
  ohlc = o ;
}
int D_EKE::Update(int N, int N1, int M1, int N2)
{
  x = ohlc->x;
  SEE_EKE(x, x,
          &ohlc->H[0], &ohlc->L[0], &ohlc->C[0],
          &preH, &preL, &preF,
          N, N1, M1, N2,
          &_R[0], &_E[0], &K[0], &E[0]) ;

  if(x>2) {
    Kx = K[x] - K[x-1] ;
    Ex = E[x] - E[x-1] ;

    if(Kx > 0) {
      if((K[x-1]-K[x-2])>0) {
        Kc = 2;  // ---  表示一直是向上
      } else {
        Kc = 1;  // ---  表示从向下变成向上
      }
    } else {
      if((K[x-1]-K[x-2])<0) {
        Kc = -2; // ---  表示一直向下
      } else {
        Kc = -1; // ---  表示从向上变为向下
      }
    }

    if(Ex > 0) {
      if((E[x-1]-E[x-2])>0) {
        Ec = 2;  // ---  表示一直是向上
      } else {
        Ec = 1;  // ---  表示从向上变成向上
      }
    } else {
      if((E[x-1]-E[x-2])<0) {
        Ec = -2; // ---  表示一直向下
      } else {
        Ec = -1; // ---  表示从向上变为向下
      }
    }
  } //---


  if(K[x]<20) {
    Kp = 0 ;
  } else if(K[x]>=20 && K[x]<=50) {
    Kp = 1 ;
  } else if(K[x]>50 && K[x]<=80) {
    Kp = 2 ;
  } else {
    Kp = 3 ;
  }

  if(E[x]<20) {
    Ep = 0 ;
  } else if(E[x]>=20 && E[x]<=50) {
    Ep = 1 ;
  } else if(E[x]>50 && E[x]<=80) {
    Ep = 2 ;
  } else {
    Ep = 3 ;
  }

  return 0;
}
//--------D_EKE end -------------------------------------------------------
D_KDJ::D_KDJ(D_OHLC * o):
  R(100000,SEE_NULL),
  K(100000,SEE_NULL),
  D(100000,SEE_NULL),
  J(100000,SEE_NULL),
  E(100000,SEE_NULL)
{
  preH = SEE_NULL;
  preL = SEE_NULL;
  preF = SEE_NULL;
  x = -1;
  ohlc = o ;
}
/*
  x: index;
*/
int D_KDJ::Update(int N, int M1, int M2, int N2)
{
  x = ohlc->x;
  SEE_KDJ(ohlc->x, ohlc->x,
          &ohlc->H[0], &ohlc->L[0], &ohlc->C[0],
          &preH, &preL, &preF,
          N, M1, M2,
          &R[0], &K[0], &D[0], &J[0]) ;
  SEE_EMA(ohlc->x, ohlc->x, &K[0], N2, &E[0]) ;


  if(ohlc->x>2) {
    Kx = K[ohlc->x] - K[ohlc->x-1] ;
    Dx = D[ohlc->x] - D[ohlc->x-1] ;
    Jx = J[ohlc->x] - J[ohlc->x-1] ;
    Ex = E[ohlc->x] - E[ohlc->x-1] ;
    if(Kx > 0) {
      if((K[ohlc->x-1]-K[ohlc->x-2])>0) {
        Kc = 2;  // ---  表示一直是向上
      } else {
        Kc = 1;  // ---  表示从向下变成向上
      }
    } else {
      if((K[ohlc->x-1]-K[ohlc->x-2])<0) {
        Kc = -2; // ---  表示一直向下
      } else {
        Kc = -1; // ---  表示从向上变为向下
      }
    }

    if(Dx > 0) {
      if((D[ohlc->x-1]-D[ohlc->x-2])>0) {
        Dc = 2;  // ---  表示一直是向上
      } else {
        Dc = 1;  // ---  表示从向上变成向上
      }
    } else {
      if((D[ohlc->x-1]-D[ohlc->x-2])<0) {
        Dc = -2; // ---  表示一直向下
      } else {
        Dc = -1; // ---  表示从向上变为向下
      }
    }

    if(Jx > 0) {
      if((J[ohlc->x-1]-J[ohlc->x-2])>0) {
        Jc = 2;  // ---  表示一直是向上
      } else {
        Jc = 1;  // ---  表示从向上变成向上
      }
    } else {
      if((J[ohlc->x-1]-J[ohlc->x-2])<0) {
        Jc = -2; // ---  表示一直向下
      } else {
        Jc = -1; // ---  表示从向上变为向下
      }
    }

    if(Ex > 0) {
      if((E[ohlc->x-1]-E[ohlc->x-2])>0) {
        Ec = 2;  // ---  表示一直是向上
      } else {
        Ec = 1;  // ---  表示从向上变成向上
      }
    } else {
      if((E[ohlc->x-1]-E[ohlc->x-2])<0) {
        Ec = -2; // ---  表示一直向下
      } else {
        Ec = -1; // ---  表示从向上变为向下
      }
    }
  } //---


  if(K[ohlc->x]<20) {
    Kp = 0 ;
  } else if(K[ohlc->x]>=20 && K[ohlc->x]<=50) {
    Kp = 1 ;
  } else if(K[ohlc->x]>50 && K[ohlc->x]<=80) {
    Kp = 2 ;
  } else {
    Kp = 3 ;
  }

  if(D[ohlc->x]<20) {
    Dp = 0 ;
  } else if(D[ohlc->x]>=20 && D[ohlc->x]<=50) {
    Dp = 1 ;
  } else if(D[ohlc->x]>50 && D[ohlc->x]<=80) {
    Dp = 2 ;
  } else {
    Dp = 3 ;
  }

  if(J[ohlc->x]<20) {
    Jp = 0 ;
  } else if(J[ohlc->x]>=20 && J[ohlc->x]<=50) {
    Jp = 1 ;
  } else if(J[ohlc->x]>50 && J[ohlc->x]<=80) {
    Jp = 2 ;
  } else {
    Jp = 3 ;
  }

  if(E[ohlc->x]<20) {
    Ep = 0 ;
  } else if(E[ohlc->x]>=20 && E[ohlc->x]<=50) {
    Ep = 1 ;
  } else if(E[ohlc->x]>50 && E[ohlc->x]<=80) {
    Ep = 2 ;
  } else {
    Ep = 3 ;
  }

  return 0;
}

// G_KDJ，将4个级别的KDJ组合了。
// 级别之间有相关性操作，所以放在一起。
//--------- G_KDJ --------------------------------
G_KDJ::G_KDJ(D_KDJ *kdj1, D_KDJ *kdj2, D_KDJ *kdj3, D_KDJ *kdj4):
  Eu1(100,SEE_NULL),  // vector 数量为100,用于记录 金叉时的位置
  Eu2(100,SEE_NULL),
  Eu3(100,SEE_NULL),
  Eu4(100,SEE_NULL),
  Ed1(100,SEE_NULL),
  Ed2(100,SEE_NULL),
  Ed3(100,SEE_NULL),
  Ed4(100,SEE_NULL),
  Ku1(100,SEE_NULL),
  Ku2(100,SEE_NULL),
  Ku3(100,SEE_NULL),
  Ku4(100,SEE_NULL),
  Kd1(100,SEE_NULL),
  Kd2(100,SEE_NULL),
  Kd3(100,SEE_NULL),
  Kd4(100,SEE_NULL)
{
  KDJ1=kdj1;
  KDJ2=kdj2;
  KDJ3=kdj3;
  KDJ4=kdj4;
  ohlc = KDJ1->ohlc ;
  eu1 = 0;   // E cross up 当第二级别处于金叉状态时，第一个级别的金叉次数
  eu2 = 0;
  eu3 = 0;
  eu4 = 0;   // eu4目前 没有用到。

  ed1 = 0;  // E  cross down 当第二级别处于金叉状态时，第一个级别的死叉次数
  ed2 = 0;
  ed3 = 0;
  ed4 = 0;

  ku1 = 0;   // K 当第二级别处于金叉状态时，第一个级别的金叉次数
  ku2 = 0;
  ku3 = 0;
  ku4 = 0;

  kd1 = 0;  // K 当第二级别处于金叉状态时，第一个级别的死叉次数
  kd2 = 0;
  kd3 = 0;
  kd4 = 0;
}

int G_KDJ::G_KDJ::Update()
{
  //---------------- K -----------------------
  if(KDJ2->Kc ==1 || KDJ2->Kc ==-1) {   // 如果第二个级别 由下变上，或者 由上变下
    Ku1[0] = Ku1[ku1];  //保留2级别最后一个拐点
    Kd1[0] = Kd1[kd1];  //保留2级别最后一个拐点
    ku1=0;              //重新计数
    kd1=0;
  }
  if(KDJ3->Kc ==1 || KDJ3->Kc ==-1) {
    Ku2[0] = Ku2[ku2];
    Kd2[0] = Kd2[kd2];
    ku2=0;
    kd2=0;
  }
  if(KDJ4->Kc ==1 || KDJ4->Kc ==-1) {
    Ku3[0] = Ku3[ku3];
    Kd3[0] = Kd3[kd3];
    ku3=0;
    kd3=0;
  }

  //if(KDJ2->Kc ==2 || KDJ2->Kc ==-2) {
  if(KDJ1->Kc ==1) {
    ku1+=1;
    Ku1[ku1] = ohlc->C[ohlc->x] ;  //只记录了 收盘价， 其实还应该记录KDJ的J值。位置x等信息
  }
  if(KDJ1->Kc ==-1) {
    kd1+=1;
    Kd1[kd1] = ohlc->C[ohlc->x] ;
  }
  //}

  //if(KDJ3->Kc ==2 || KDJ3->Kc ==-2) {
  if(KDJ2->Kc ==1) {
    ku2+=1;
    Ku2[ku2] = ohlc->C[ohlc->x] ;
  }
  if(KDJ2->Kc ==-1) {
    kd2+=1;
    Kd2[kd2] = ohlc->C[ohlc->x] ;
  }
  //}

  //if(KDJ4->Kc ==2 || KDJ4->Kc ==-2) {
  if(KDJ3->Kc ==1) {
    ku3+=1;
    Ku3[ku3] = ohlc->C[ohlc->x] ;
  }
  if(KDJ3->Kc ==-1) {
    kd3+=1;
    Kd3[kd3] = ohlc->C[ohlc->x] ;
  }
  //}

  // KDJ2->Ec 表示 当 KDJ的E值 开始从下向上时，Ec为1，当E值一直向上时，Ec为2，
  // 当E值由上转下时，为-1，当一直为下时，Ec为-2 ...
  //---------------- E -----------------------
  if(KDJ2->Ec ==1 || KDJ2->Ec ==-1) {
    Eu1[0] = Eu1[eu1];  //保留2级别最后一个拐点
    Ed1[0] = Ed1[ed1];  //保留2级别最后一个拐点
    eu1=0;
    ed1=0;
  }
  if(KDJ3->Ec ==1 || KDJ3->Ec ==-1) {
    Eu2[0] = Eu2[eu2];
    Ed2[0] = Ed2[ed2];
    eu2=0;
    ed2=0;
  }
  if(KDJ4->Ec ==1 || KDJ4->Ec ==-1) {
    Eu3[0] = Eu3[eu3];
    Ed3[0] = Ed3[ed3];
    eu3=0;
    ed3=0;
  }

  //if(KDJ2->Ec ==2 || KDJ2->Ec ==-2) {
  if(KDJ1->Ec ==1) {
    eu1+=1;
    Eu1[eu1] = ohlc->C[ohlc->x] ;
  }
  if(KDJ1->Ec ==-1) {
    ed1+=1;
    Ed1[ed1] = ohlc->C[ohlc->x] ;
  }
  //}

  //if(KDJ3->Ec ==2 || KDJ3->Ec ==-2) {
  if(KDJ2->Ec ==1) {
    eu2+=1;
    Eu2[eu2] = ohlc->C[ohlc->x] ;
  }
  if(KDJ2->Ec ==-1) {
    ed2+=1;
    Ed2[ed2] = ohlc->C[ohlc->x] ;
  }
  //}

  //if(KDJ4->Ec ==2 || KDJ4->Ec ==-2) {
  if(KDJ3->Ec ==1) {
    eu3+=1;
    Eu3[eu3] = ohlc->C[ohlc->x] ;
  }
  if(KDJ3->Ec ==-1) {
    ed3+=1;
    Ed3[ed3] = ohlc->C[ohlc->x] ;
  }
  //}

} //-----update----

} // namespace

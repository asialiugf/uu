#ifndef UBEE_INDEX_H
#define UBEE_INDEX_H

#include "see_talib.h"
#include "uBEE.h"
#include <stdlib.h>
#include <string.h>
#include <vector>

namespace uBEE
{
struct D_OHLC {
  std::vector<double> O;
  std::vector<double> H;
  std::vector<double> L;
  std::vector<double> C;
  std::vector<int> V;
  char cB[9];
  char cE[9];
  int  x;    // index !! O[0]... O[1]... O[x] ...
public:
  D_OHLC();
  int Insert(sKbar * bar);
};

//------------------------------------------------------------

struct D_RSI {
  D_OHLC             *ohlc ;
  std::vector<double> _max;
  std::vector<double> _abs;
  std::vector<double> _sma1;
  std::vector<double> _sma2;
  std::vector<double> RSI;
  int 				x ;
  int               ls ;
public:
  D_RSI(D_OHLC *o);
  int Update(int N);
};

//--begin-------------------EKE (_R:=RSV(N)  _E:=EMA(_R,N1)  K:=SMA(_E,M1,1)  E:=EMA(K,N2) -----------
struct D_EKE {
private:
  std::vector<double>    _R;
  std::vector<double>    _E;
  double                 preH;
  double                 preL;
  int                    preF;
public:
  std::vector<double>    K;
  std::vector<double>    E;
  D_OHLC               * ohlc ;
  int                    x ;

  // ---- 上涨下跌
  double                 Kx ;  // = K[x] - K[x-1]
  double                 Ex ;
  int                    Kc ;   // K cross
  int                    Ec ;   // --- E cross up Ec=1, Ec=2 -------------- Ec = -1, Ec=-2 -----------
  // ---- 在哪个区间
  int                    Kp ; // <20 |  20< <50 |  50< <80  | 80<
  int                    Ep ;

public:
  D_EKE(D_OHLC *o);
  int Update(int N, int N1, int M1, int N2);
};
//--end-------------------EKE (_R:=RSV(N)  _E:=EMA(_R,N1)  K:=SMA(_E,M1,1)  E:=EMA(K,N2) -----------

//-------------------------------KDJ-------------------
struct D_KDJ {
  std::vector<double>    R;
  std::vector<double>    K;
  std::vector<double>    D;
  std::vector<double>    J;
  std::vector<double>    E;
  double                 preH;
  double                 preL;
  int                    preF;
  D_OHLC               * ohlc ;
  int                    x ;
  // ---- 上涨下跌
  double                 Kx ;  // = K[x] - K[x-1]
  double                 Dx ;
  double                 Jx ;
  double                 Ex ;
  int  Kc ;   // K cross
  int  Dc ;
  int  Jc ;
  int  Ec ;            // --- E cross up Ec=1, Ec=2 -------------- Ec = -1, Ec=-2 -----------
  // ---- 在哪个区间
  int                    Kp ; // <20 |  20< <50 |  50< <80  | 80<
  int                    Dp ;
  int                    Jp ;
  int                    Ep ;
  // ---- 大级别内 紧临 小级别 的第几次拐头
  //int

public:
  D_KDJ(D_OHLC *o);
  int Update(int N, int M1, int M2, int N2);
};


//-------------------------------STC-------------------
struct D_STC {
  D_KDJ                 *kdj ;
  double                 preH;
  double                 preL;
  int                    preF;
  std::vector<double>    STC;
  int                    x ;
public:
  D_STC(D_KDJ *k);
  int Update(int N);
};

//----begin-------------------------- MNF magnetic force -----------------
struct D_MNF {
  D_EKE                 *KE;
  double                 preH;
  double                 preL;
  int                    preF;
  std::vector<double>    MNF;
  int                    x ;
  // -------
  //  当MNF >=100时，  ud = 100 ;  贴上边了。
  //  当MNF从 100开始向下时，ud = 99 ; 离开上边了。
  //  当MNF从 0开始向上时，ud = 1 ; 离开下边了。
  //  当MNF <=0 时，   ud = 0 ;  贴下边了。
  int                    ud ;   // 记录起始点  up down ==> ud;
  int                    udN ;
  int                    uN ;   // up Number ; 记录时间长度
  int                    dN ;   // down Number ;

  int                    ls ;   // long short 
  int                    lsN ;  // long short number ;

public:
  D_MNF(D_EKE *e);
  int Update(int N);
};
//----end--------------------------  MNF magnetic force -----------------


struct G_KDJ {
  D_OHLC      * ohlc ;
  D_KDJ       * KDJ1 ;
  D_KDJ       * KDJ2 ;
  D_KDJ       * KDJ3 ;
  D_KDJ       * KDJ4 ;

  int  eu1,eu2,eu3,eu4;   // E cross up 当第二级别处于金叉状态时，第一个级别的金叉次数
  int  ed1,ed2,ed3,ed4;   // E  cross down 当第二级别处于金叉状态时，第一个级别的死叉次数
  std::vector<double>    Eu1;   // 用于记录上涨 的位置，第0次 第1次，第 [eu1] 次
  std::vector<double>    Eu2;
  std::vector<double>    Eu3;
  std::vector<double>    Eu4;
  std::vector<double>    Ed1;   // 用于记录下跌的位置，Ed1[ed1] > Ed1[ed1-1] 表示第ed1次下降时，比第ed1-1（前一次）要高。
  std::vector<double>    Ed2;
  std::vector<double>    Ed3;
  std::vector<double>    Ed4;

  int  ku1,ku2,ku3,ku4;   // K value up --- K 当第二级别处于金叉状态时，第一个级别的金叉次数
  int  kd1,kd2,kd3,kd4;   // K value down --- K 当第二级别处于金叉状态时，第一个级别的死叉次数
  std::vector<double>    Ku1;
  std::vector<double>    Ku2;
  std::vector<double>    Ku3;
  std::vector<double>    Ku4;
  std::vector<double>    Kd1;
  std::vector<double>    Kd2;
  std::vector<double>    Kd3;
  std::vector<double>    Kd4;

  G_KDJ(D_KDJ *kdj1, D_KDJ *kdj2, D_KDJ *kdj3, D_KDJ *kdj4);
  int Update() ;

}; //-----------


} //namespace

#endif // UBEE_INDEX_H

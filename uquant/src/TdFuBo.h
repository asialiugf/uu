#ifndef UBEE_TDRATE_H
#define UBEE_TDRATE_H

#include <uWS/uWS.h>
//#include <uBEE.h>
#include "Bars.h"
#include "SigSlot.h"
#include <thread>
#include <mutex>
#include <condition_variable>
#include <functional>
#include <vector>
#include <queue>

namespace uBEE
{
using lSignal::Connection;
using lSignal::Signal;
using lSignal::Slot;

//--------------------------------------------------------------------------
struct sOHLC {
  char cB[9];
  char cE[9];
  std::vector<double> O;
  std::vector<double> H;
  std::vector<double> L;
  std::vector<double> C;
  std::vector<int> V;
  int  x;       // index !! O[0]... O[1]... O[x] ...
  int  u;       // update bar  u:=0 表示真正结束的bar， u:=1 表示这是一个没有结束的bar。 u:=-1 表示第一个，新开始。
public:
  sOHLC();
  int Insert(sKbar * bar);
  int Update(sKbar * bar);
};

//--------------------------------------------------------------------------
/*
  M_FuRate  M_FuRate 定义每个期货品种的 最小变动价格等
  double          mMPF ;             // 最小变动价位  Tick Size, Minimum Price Fluctuation  比如橡胶是5元
  double          mLot ;             // 每手收益 比如 一手橡胶 是50元
  double          mOP ;              // money for open  position 开仓手续费
  double          mCP ;              // money for close position 平仓手续费
  double          LP ;               // long position 多头头寸
  double          SP ;               // short position 空头头寸
*/

static const std::map<std::string,std::string> M_FuRate = {
  //  czce 郑商所 -------801 4位------------
  {"AP", "mMPF:5 mLot:50 mOP:7 mCP:7"   }, // 苹果
  {"CF", "mMPF:5 mLot:50 mOP:7 mCP:7"   }, // 棉花
  {"CY", "mMPF:5 mLot:50 mOP:7 mCP:7"   }, // 棉纱
  {"FG", "mMPF:5 mLot:50 mOP:7 mCP:7"   }, // 玻璃
  {"JR", "mMPF:5 mLot:50 mOP:7 mCP:7"   }, // 粳稻
  {"LR", "mMPF:5 mLot:50 mOP:7 mCP:7"   }, // 晚籼稻
  {"MA", "mMPF:5 mLot:50 mOP:7 mCP:7"   }, // 甲醇N
  {"OI", "mMPF:5 mLot:50 mOP:7 mCP:7"   }, // 菜籽油
  {"PM", "mMPF:5 mLot:50 mOP:7 mCP:7"   }, // 普麦
  {"RI", "mMPF:5 mLot:50 mOP:7 mCP:7"   }, // 早籼稻
  {"RM", "mMPF:5 mLot:50 mOP:7 mCP:7"   }, // 菜籽粕
  {"RS", "mMPF:5 mLot:50 mOP:7 mCP:7"   }, // 油菜籽
  {"SF", "mMPF:5 mLot:50 mOP:7 mCP:7"   }, // 硅铁
  {"SM", "mMPF:5 mLot:50 mOP:7 mCP:7"   }, // 锰硅
  {"SR", "mMPF:5 mLot:50 mOP:7 mCP:7"   }, // 白糖
  {"TA", "mMPF:5 mLot:50 mOP:7 mCP:7"   }, // PTA
  {"WH", "mMPF:5 mLot:50 mOP:7 mCP:7"   }, // 强麦
  {"ZC", "mMPF:5 mLot:50 mOP:7 mCP:7"   }, // 新动力煤
  // {"TC",  2   }, // 动力煤
  // {"ME",  0   }, // 甲醇
  //  dce 大商所 --- 16 --- 20180109 ----------- 1801 4---------------------------------------------
  {"a", "mMPF:5 mLot:50 mOP:7 mCP:7"  },//   豆一
  {"b", "mMPF:5 mLot:50 mOP:7 mCP:7"  },//   豆二
  {"bb","mMPF:5 mLot:50 mOP:7 mCP:7"  },//   胶合板
  {"c", "mMPF:5 mLot:50 mOP:7 mCP:7"  },//   玉米
  {"cs","mMPF:5 mLot:50 mOP:7 mCP:7"  },//   玉米淀粉
  {"fb","mMPF:5 mLot:50 mOP:7 mCP:7"  },//   纤维板
  {"i", "mMPF:5 mLot:50 mOP:7 mCP:7"  },//   铁矿石
  {"j", "mMPF:5 mLot:50 mOP:7 mCP:7"  },//   焦炭
  {"jd","mMPF:5 mLot:50 mOP:7 mCP:7"  },//   鸡蛋
  {"jm","mMPF:5 mLot:50 mOP:7 mCP:7"  },//   焦煤
  {"l", "mMPF:5 mLot:50 mOP:7 mCP:7"  },//   聚乙烯
  {"m", "mMPF:5 mLot:50 mOP:7 mCP:7"  },//   豆粕
  {"p", "mMPF:5 mLot:50 mOP:7 mCP:7"  },//   棕榈油
  {"pp","mMPF:5 mLot:50 mOP:7 mCP:7"  },//   聚丙烯
  {"v", "mMPF:5 mLot:50 mOP:7 mCP:7"  },//   聚氯乙烯
  {"y", "mMPF:5 mLot:50 mOP:7 mCP:7"  },//   豆油
  // shfe  上期所--- 14 --- 20180109 -----------------------
  {"cu","mMPF:5 mLot:50 mOP:7 mCP:7" }, //   铜
  {"al","mMPF:5 mLot:50 mOP:7 mCP:7" }, //   铝
  {"zn","mMPF:5 mLot:50 mOP:7 mCP:7" }, //   锌
  {"pb","mMPF:5 mLot:50 mOP:7 mCP:7" }, //   铅
  {"ni","mMPF:5 mLot:50 mOP:7 mCP:7" }, //   镍
  {"sn","mMPF:5 mLot:50 mOP:7 mCP:7" }, //   锡
  {"au","mMPF:5 mLot:50 mOP:7 mCP:7" }, //   黄金
  {"ag","mMPF:5 mLot:50 mOP:7 mCP:7" }, //   银
  {"rb","mMPF:5 mLot:50 mOP:7 mCP:7" }, //   螺纹钢
  {"wr","mMPF:5 mLot:50 mOP:7 mCP:7" }, //   线材
  {"hc","mMPF:5 mLot:50 mOP:7 mCP:7" }, //   热轧卷板
  {"fu","mMPF:5 mLot:50 mOP:7 mCP:7" }, //   燃料油
  {"bu","mMPF:5 mLot:50 mOP:7 mCP:7" }, //   石油沥青
  {"ru", "mMPF:5 mLot:50 mOP:7 mCP:7" }, //   天然橡胶
  //cffe 中金所 --- 5 --- 20180109 -------------------------
  {"IF", "mMPF:5 mLot:50 mOP:7 mCP:7" }, // 沪深300
  {"IC", "mMPF:5 mLot:50 mOP:7 mCP:7" }, // 中证500
  {"IH", "mMPF:5 mLot:50 mOP:7 mCP:7" }, // 上证50
  {"TF", "mMPF:5 mLot:50 mOP:7 mCP:7" }, // 国债5年期
  {"T",  "mMPF:5 mLot:50 mOP:7 mCP:7" }, // 国债10年期
};


// Future storage for  future and periods!
// the memory image:
//  "ru1805"
//  iP[0] = -1;
//  ip[1] = -1;
//  ip[2] = -1;
//  ip[3] = -1;
//  ip[4] = 5;     // index 4 === period is 5 seconds//  ...
//  ip[x] = 60;    // index x === period is 60 seconds.
//  ...
//  ip[49] = -1;
// mainhub.onMessage will set bars[0] bars[1]
// bars[0] ==> ip[4] = 5;
// bars[1] ==> ip[x] = 60;
// ip[n] noused will be set to  -1 ;
// refer to FuInit() ! | index 0 for tick. index 1----30 basic periods.  index 31-49 for custom define .
/*
  strategy future block !!!!!
*/
struct sFuBo {
  char            InstrumentID[31];
  char            ID2[3] ;           // "zz zn cu ..."
  int             iP[50] ;           //period
  sOHLC          *pBars[50];         //用于存储 kbar数据
  Signal<void()>  Update[50];        // signal slot 这是一个信号
  double          mMPF ;             // 最小变动价位  Tick Size, Minimum Price Fluctuation  比如橡胶是5元
  double          mLot ;             // 每手收益 比如 一手橡胶 是50元
  double          mOP ;              // money for open position 开仓手续费
  double          mCP ;              // money for close position 平仓手续费
  double          LP ;               // long position 多头头寸
  double          SP ;               // short position 空头头寸
  double          BL ;
  double          BS ;
  double          SL ;
  double          SS ;
  int 	          NL ;
  int             NS ;
  double          mPL ;      // 盈亏 profit and loss
  double          cPL ;      // 盈亏 current profit and loss
public:
  sFuBo(std::string);
  int BuyShort(int n,double c);  //手数，收盘价
  int BuyLong(int n,double c);
  int SellShort(int n,double c);
  int SellLong(int n,double c);
  int StopLost(int n, double c);
  int DStopLost(int n, double c);
  int StopProfit(int n, double c);
  int CurrPL(double c);
};



} //namespace

#endif // UBEE_TDRATE_H

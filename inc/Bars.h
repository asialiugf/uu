#ifndef UBEE_BARS_H
#define UBEE_BARS_H

#include "../ctp/ThostFtdcMdApi.h"
//#include "Psqlpool.h"
//#include "PsqlFunc.h"
#include <uWS/uWS.h>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <functional>
#include <vector>
#include <queue>

namespace uBEE
{

#define SAVE_ALL 1
#define SAVE_BAR 2
#define SAVE_TICK 3

#define SEND_BAR 4
#define SEND_ALL 5
#define SEND_TICK 6

#define SEND_SAVE_ALL 7
#define SEND_SAVE_BAR 8
#define SEND_SAVE_TICK 9


// #define T________  1

#define see_memzero(buf, n)         (void) memset(buf, 0, n)
#define SEE_NULL    -2147483648
#define  SEE_OK          0

#define  BAR_SECOND        's'
#define  BAR_MINUTE        'f'
#define  BAR_HOUR          'h'
#define  BAR_DAY           'd'
#define  BAR_WEEK          'w'
#define  BAR_MONTH         'm'
#define  BAR_SEASON        'j'
#define  BAR_YEAR          'y'
#define  BAR_TICKC         't'

#define  FUTRUE_ID_LEN     31
#define  SEE_SGM_NUM       50
#define  SEE_HOUR_TYPE_NUM 10
//#define  TRADE_TIME         "../etc/tbl/see_trade_time"
//#define  FUTURE_DAYS        "../etc/tbl/see_future_days"
//#define  FUTURE_TIME        "../etc/tbl/see_future_time"

#define TICK 				struct CThostFtdcDepthMarketDataField

/*
   注意：
    下面的定义，与数组下标是严格一致的。
    block->bar_block[0]  = block->bar_block[BAR_TICK]
    block->bar_block[13]  = block->bar_block[BAR_10F]
    各种函数的 period 也用的是下面的定义。

*/
#define  BAR_TICK    0
#define  BAR_1S      1
#define  BAR_2S      2
#define  BAR_3S      3
#define  BAR_5S      4
#define  BAR_10S     5
#define  BAR_15S     6
#define  BAR_20S     7
#define  BAR_30S     8
#define  BAR_1F      9
#define  BAR_2F      10
#define  BAR_3F      11
#define  BAR_5F      12
#define  BAR_10F     13
#define  BAR_15F     14
#define  BAR_20F     15
#define  BAR_30F     16
#define  BAR_1H      17
#define  BAR_2H      18
#define  BAR_3H      19
#define  BAR_4H      20
#define  BAR_5H      21
#define  BAR_6H      22
#define  BAR_8H      23
#define  BAR_10H     24
#define  BAR_12H     25
#define  BAR_1D      26
#define  BAR_1W      27
#define  BAR_1M      28
#define  BAR_1J      29
#define  BAR_1Y      30

//标准周期
//  0   1   2   3   4   5    6    7    8    9    10    11    12    13    14    15     16
//  0S  1S  2S  3S  5S  10S  15S  20S  30S  1F   2F    3F    5F    10F   15F   20F    30F
//  0,  1,  2,  3,  5,  10,  15,  20,  30,  60,  120,  180,  300,  600,  900,  1200,  1800
//  0S是tick。
//  17    18    19     20     21      22   23   24   25   26   27   28   29  30
//  1H    2H    3H     4H     5H      6H   8H   10   12   1D   1W   1M   1J  1Y
//  3600, 7200, 10800, 14400, 18000,  0,   0,   0,   0,   0,   0,   0,   0,  0

// 用户自定义 交易周期 不能超过20个 不能和已有的周期重复！
//                    31  32   33   34   35
// const int fr[5] = {19,14401,9900,350,6600};

// -- 前面加上 100 101 ... 是为了排序需要.
// M_FF是为 HubCtp HubSim根据tick生成kbar用的，如果不需要，可以将后面的值改为-1即可。
// 如果想要定义自己的级别，可以在后面的131_cu 添加，值以秒计.
// 日 周 月 季 年 todo!!
static const std::map<std::string,int> M_FF = {
  {"100_T",   0      },  // 0 tick!!!
  {"101_1S",  1      },  // 1
  {"102_2S",  2      },  // 2
  {"103_3S",  3      },  // 3
  {"104_5S",  5      },  // 4
  {"105_10S", 10     },  // 5
  {"106_15S", 15     },  // 6
  {"107_20S", 20     },  // 7
  {"108_30S", 30     },  // 8
  {"109_1F",  60     },  // 9
  {"110_2F",  120    },  // 10
  {"111_3F",  180    },  // 11
  {"112_5F",  300    },  // 12
  {"113_10F", 600    },  // 13
  {"114_15F", 900    },  // 14
  {"115_20F", 1200   },  // 15
  {"116_30F", 1800   },  // 16
  {"117_1H",  3600   },  // 17
  {"118_2H",  7200   },  // 18
  {"119_3H",  10800  },  // 19
  {"120_4H",  14400  },  // 20
  {"121_5H",  18000  },  // 21
  {"122_6H",  21600  },  // 22
  {"123_8H",  28800  },  // 23
  {"124_10H", 36000  },  // 24
  {"125_12H", 43200  },  // 25
  {"126_1D",  -1   },  // 26
  {"127_1W",  -1   },  // 27
  {"128_1M",  -1   },  // 28
  {"129_1J",  -1   },  // 29
  {"130_1Y",  -1   },  // 30
  //---------- for custom define 自定义周期---------------
  {"131_cu",  -1   },
  {"132_cu",  -1   },
  {"133_cu",  -1   },
  {"134_cu",  -1   },
  {"135_xp",  3456 },
  {"136_cu",  -1   },
  {"137_cu",  -1   },
  {"138_cu",  -1   },
  {"139_cu",  -1   },
  {"140_cu",  -1   },
  {"141_cu",  -1   },
  {"142_cu",  -1   },
  {"143_cu",  -1   },
  {"144_cu",  -1   },
  {"145_cu",  -1   },
  {"146_cu",  -1   },
  {"147_cu",  -1   },
  {"148_cu",  -1   },
  {"149_cu",  -1   },  // 不能超过50个周期！！ 在 base.h <struct Future> 中 写死了。
};


#define SAME_SEG_8       8
#define SAME_SEG_9       9

#define SAME_NO_SEG_0       -1
#define SAME_NO_SEG_1       -1
#define SAME_NO_SEG_2       -2
#define SAME_NO_SEG_3       -3
#define SAME_NO_SEG_4       -4
#define SAME_NO_SEG_5       -5
#define SAME_NO_SEG_6       -6
#define SAME_NO_SEG_7       -7
#define SAME_NO_SEG_8       -8
#define SAME_NO_SEG_9       -9

#define SEE_MKT_OPEN       10000        /*  market openning */
#define SEE_MKT_CLOSE      10001        /*  market closed   */
#define SEE_SAME_SEG          10002
#define SEE_DIFF_SEG          10003
#define SEE_SAME_NO_SEG       10004
#define SEE_DIFF_NO_SEG       10005
#define SEE_SAME_KBAR         10006
#define SEE_DIFF_KBAR         10007

#define SEE_START_TICK        10008   // 开机后第一个交易时间段内的tick 


#define UPDATE_B1      b1->c = tick->LastPrice ; \
                       if ( b1->h < tick->LastPrice ) { b1->h = tick->LastPrice ; } \
                       if ( b1->l > tick->LastPrice ) { b1->l = tick->LastPrice ; } \
                       b1->vsum = tick->Volume ; \
                       b1->v = tick->Volume - b1->vold ;
/*
#define NEW_B1         memcpy(b1->ActionDay,tick->ActionDay,9) ; \
                       memcpy(b1->TradingDay,tick->TradingDay,9) ; \
*/
#define NEW_B1         b1->iX = period ; \
                       b1->iF = fubo->pBaBo[period]->iF ; \
                       b1->o = tick->LastPrice ; \
                       b1->h = tick->LastPrice ; \
                       b1->l = tick->LastPrice ; \
                       b1->c = tick->LastPrice ; \
                       b1->vold = b1->vsum; \
                       b1->vsum = tick->Volume ; \
                       b1->v = tick->Volume - b1->vold ; \
                       b1->sent = 0;


// -----Begin ----------- 交易时间定义 ----------------------------------------
#define SGM_NUM 10
static const std::map<int,std::string> M_TimeType = {
  {0,"{\"time\":[                                \"09:00-10:15\",\"10:30-11:30\",\"13:30-15:00\"]}"},
  {1,"{\"time\":[\"21:00-23:00\",                \"09:00-10:15\",\"10:30-11:30\",\"13:30-15:00\"]}"},
  {2,"{\"time\":[\"21:00-23:30\",                \"09:00-10:15\",\"10:30-11:30\",\"13:30-15:00\"]}"},
  {3,"{\"time\":[\"21:00-24:00\",\"00:00-01:00\",\"09:00-10:15\",\"10:30-11:30\",\"13:30-15:00\"]}"},
  {4,"{\"time\":[\"21:00-24:00\",\"00:00-02:30\",\"09:00-10:15\",\"10:30-11:30\",\"13:30-15:00\"]}"},
  {5,"{\"time\":[                                \"09:30-11:30\",                \"13:00-15:00\"]}"},
  {6,"{\"time\":[                                \"09:15-11:30\",                \"13:00-15:15\"]}"},
};

// ----- End ----------- 交易时间定义 ----------------------------------------

struct Segment {
  char cB[9];
  char cE[9];
  int  iB ;
  int  iE ;
  int  iI ;  //和前一个 segment之间的间隔。如果是第一个segment， iI = 0;
  int  mark ; // 0 表示 seg 包含 bar。 // 1.2.3. 表示 一个bar 包含多个 seg。
  int  sn ;
  char barB[9];
  char barE[9];
  int  bariB;
  int  bariE;
  int  barBx;  //记录 起始barB所在的 seg[]的 index .
  int  barEx;  //记录 结束barE所在的 seg[]的 index .
public:
  Segment() ;
};

struct stTimeType {
  int        iType;  /* 不同的交易时间类型 */
  int        iSegNum; /* 多少个segment */
  Segment    aSgms[SGM_NUM] ;
};

// time block ----------
struct TmBo {
  stTimeType TT[7] ;   // 有7种交易时间类型。参见 M_TimeType
public:
  TmBo();
private:
  int Init(stTimeType TT[]) ;
};

// ----- End ----------- 时间结构定义 ----------------------------------------

struct stBar {
  int     iX;              // 索引号  0:TICK  1:2S 2:3S 4:5S ...   100:tick=0;
  int     iF;              // 周期  600 300 ...：:w
  char    cB[9];   	   // begin time BAR K柱 的开始时间
  char    cE[9];   	   // end time
  double  o ;             // open
  double  h ;             // high
  double  l ;             // low
  double  c ;             // close
  int     v ;             // volume
  int     vsum ;          // keep volume sum
  int     vold ;          // 前一个K柱的结束时的量
  int     sent ;          // 有没有sent 或者写入  ==2 已写 ==1 已sent 但没有写 ==0 没发没写
};

struct BaBo {
  stBar         bar1 ;
  stBar        *b1 ;
  char          curB[9];            //记录当前tick所在的段
  char          curE[9];
  int           curiB ;             //记录当前tick所在的段
  int           curiE ;
  int           curiX ;              //记录当前tick所在的段的下标
  int           iBidx;              // 此bar1的起始点所在的 segment idx，
  int           iEidx;              // 此bar1的结束点所在的 segment idx，
  int           iF;                 // 0:tick 1:2s, 60:1F ---- 周期 ： 以秒计
  int           iH;                 // 将周期转成 H M S, 比如：周期为 97 秒 转成： iPeriodH=0 ;
  int           iM;                 // iPeriodM = 1;
  int           iS;                 // iPeriodS = 37;  需要初始化！！
  char          cF[10] ;             // frequency ; "1S" "2S" ...
  char          ca_table[128];       /* database table name */
  int           iSegNum ;          // segment 数量
  Segment      *seg[100] ;        // segment array
public:
  BaBo(const char *pF, int fr, stTimeType  *pTimeType);
  //int MakeTime(char *caTime, int T) ;
} ;

struct FuBo {
  //std::shared_ptr<uBEE::DBPool> dbpool;
  uWS::Group<uWS::SERVER> * SG;
  uWS::WebSocket<uWS::SERVER> *ws ;         // HubBck 需要。
  int          iChange ;                    // Tick changed "20:59:??" to "21:00:00"
  char         InstrumentID[31];
  char         ActionDay[9];
  char         TradingDay[9];
  char         UpdateTime[9] ;              // for saving "20:59:??" .....
  char         caFileName[1024];            // 用于记录"/home/rabbit/see/dat/rcv_dat/au/au1801",要组合 period
  int          iTickValid ;                    // 用于记录收到tick时，是否有效
  stTimeType  *pTimeType ;                  // TimeType
  BaBo        *pBaBo[50] ;                 // 1s 2s 3s ... 1f 2f 3f 5f ... 1h 5h ... 1y tick
public:
  FuBo(char *fuID, uBEE::TmBo *tmbo, uWS::Group<uWS::SERVER> *sg);
};


// -------------------- for sending -----------------

struct sTick {                 // tick for send !!
  int     iType ;                 // type :  0:tick  1 2 3 ... for other ....
  char    InstrumentID[31];
  char    ActionDay[9];
  char    TradingDay[9];
  //int     iX;              // 索引号  0:1S  1:2S 2:3S 4:5S ...   100:tick=0;
  //int     iF;              // 周期  600 300 ...：:w
  char          UpdateTime[9];          ///最后修改时间
  double        OpenPrice;              ///今开盘
  double        HighestPrice;           ///最高价
  double        LowestPrice;            ///最低价
  double        LastPrice;              ///最新价
  double        OpenInterest;           ///持仓量
  int           UpdateMillisec;         ///最后修改毫秒
  double        BidPrice1;              ///申买价一
  int           BidVolume1;             ///申买量一
  double        AskPrice1;              ///申卖价一
  int           AskVolume1;             ///申卖量一
  int           Volume;                 ///数量
};

struct sKbar {             // Kbar for send !!
  int     iX;              // 索引号  0:1S  1:2S 2:3S 4:5S ...   100:tick=0;
  int     iF;              // 周期  600 300 ...：:w
  char    cB[9];           // begin time BAR K柱 的开始时间
  char    cE[9];           // end time
  double  o ;              // open
  double  h ;              // high
  double  l ;              // low
  double  c ;              // close
  int     v ;              // volume
  int     vsum ;           // keep volume sum
};

struct sData {
  int     iType ;                 // type :  0:tick  1 2 3 ... for other ....  1:nomal 2:update
  char    InstrumentID[31];
  char    ActionDay[9];
  char    TradingDay[9];
  int     iN ;                 // number of KK below !!
  sKbar   KK[50];
};

static const int hLen = sizeof(sData) - 50 * sizeof(sKbar) ;   // head length
static const int tLen = sizeof(sTick) ;                        // tick length
static const int bLen = sizeof(sKbar) ;                        // bar length
static const int oLen = hLen + bLen ;

// --- define for sData iType
#define T_TICK    0
#define T_BARS    1
#define T_UPDATE  2


// ----- End ----------- 时间结构定义 ----------------------------------------



int MakeTime(char *caT, int T) ;

int SendTick(uBEE::FuBo *fubo, TICK *tick);
int SaveTick(uBEE::FuBo *fubo, TICK *tick);
int HandleTick(uBEE::FuBo *fubo, TICK *tick,            int flag);
int MarkBar(uBEE::FuBo *fubo, TICK *tick,int period);
int SendBar(uBEE::FuBo *fubo,            int period, int flag);
int SaveBar(uBEE::FuBo *fubo, sKbar *KK ,int period);
int DealBar(uBEE::FuBo *fubo, TICK *tick,int period, int flag);

int Display(uBEE::FuBo *fubo, TICK *tick,int period,const char*msg);
int DispBar(uBEE::FuBo *fubo, TICK *tick,int period,const char*msg);
int DispKbar(const char *InstrumentID, const char *TradingDay, const char *ActionDay, sKbar * bar);
int GetFrequencyIdx(int f);


//--------------------------------------------------

struct X_OHLC {
  char cB[9];
  char cE[9];
  std::vector<double> O;
  std::vector<double> H;
  std::vector<double> L;
  std::vector<double> C;
  std::vector<int> V;
  int  x;       // index !! O[0]... O[1]... O[x] ...
  int  u;       // update bar  u:=0 表示真正结束的bar， u:=1 表示这是一个没有结束的bar。 u:=-1 表示第一个，新开始。
  int  iCall ;  // callbacks数量.
  //void (*callbacks[256])();
  std::function<void()> callbacks[256] ;
public:
  X_OHLC();
  int Insert(sKbar * bar);
  int Update(sKbar * bar);
};


} //end namespace
#endif // UBEE_BARS_H

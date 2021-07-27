#ifndef UBEE_BARS_H
#define UBEE_BARS_H

#include "../ctp/ThostFtdcMdApi.h"
#include "Psqlpool.h"
#include "PsqlFunc.h"
#include <uWS/uWS.h>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <functional>
#include <vector>
#include <queue>

namespace uBEE
{

#define T________  1

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
#define  TRADE_TIME         "../etc/tbl/see_trade_time"
#define  FUTURE_DAYS        "../etc/tbl/see_future_days"
#define  FUTURE_TIME        "../etc/tbl/see_future_time"

#define TICK 				struct CThostFtdcDepthMarketDataField

/*
   注意：
    下面的定义，与数组下标是严格一致的。
    block->bar_block[0]  = block->bar_block[BAR_1S]
    block->bar_block[13]  = block->bar_block[BAR_15F]
    block->bar_block[0]  = block->bar_block[BAR_1S]
    block->bar_block[13]  = block->bar_block[BAR_15F]
    各种函数的 period 也用的是下面的定义。

*/
#define  BAR_1S       0
#define  BAR_2S       1
#define  BAR_3S       2
#define  BAR_5S       3
#define  BAR_10S      4
#define  BAR_15S      5
#define  BAR_20S      6
#define  BAR_30S      7
#define  BAR_1F       8
#define  BAR_2F       9
#define  BAR_3F       10
#define  BAR_5F       11
#define  BAR_10F      12
#define  BAR_15F      13
#define  BAR_20F      14
#define  BAR_30F      15
#define  BAR_1H       16
#define  BAR_2H       17
#define  BAR_3H       18
#define  BAR_4H       19
#define  BAR_5H       20
#define  BAR_6H       21
#define  BAR_8H       22
#define  BAR_10H      23
#define  BAR_12H      24
#define  BAR_1D       25
#define  BAR_1W       26
#define  BAR_1M       27
#define  BAR_1J       28
#define  BAR_1Y       29
#define  BAR_TICK     30

/*                        0  1 2 3 4  5  6  7  8  9   10  11  12  13  14   15   16   17   18    19    20    21 22 23 24 25 26 27 28 29 */
/*                        1S 2 3 5 10 15 20 30 1F 2F  3F  5F  10  15  20   30   1H   2H   3H    4H    5H    6H 8H 10 12 1D 1W 1M 1J 1Y */
/*
static const int ff[30] = {1, 2,3,5,10,15,20,30,60,120,180,300,600,900,1200,1800,3600,7200,10800,14400,18000, 0, 0, 0, 0, 0, 0, 0, 0, 0};
*/

// -- 前面加上 100 101 ... 是为了排序需要.
static const std::map<std::string,int> M_FF = {
  {"100_0S",  0      },   // tick!!!
  {"100_1S",  1      },
  {"101_2S",  2      },
  {"102_3S",  3      },
  {"103_5S",  5      },
  {"104_10S", 10     },
  {"105_15S", 15     },
  {"106_20S", 20     },
  {"107_30S", 30     },
  {"108_1F",  60     },
  {"109_2F",  120    },
  {"110_3F",  180    },
  {"111_5F",  300    },
  {"112_10F", 600    },
  {"113_15F", 900    },
  {"114_20F", 1200   },
  {"115_30F", 1800   },
  {"116_1H",  3600   },
  {"117_2H",  7200   },
  {"118_3H",  10800  },
  {"119_4H",  14400  },
  {"120_5H",  18000  },
  {"121_6H",  21600  },
  {"122_8H",  28800  },
  {"123_10H", 36000  },
  {"124_12H", 43200  },
  {"125_1D",  0   },
  {"126_1W",  0   },
  {"127_1M",  0   },
  {"128_1J",  0   },
  {"129_1Y",  0   },
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


#define MOVE_BAR       p_bar0->o = p_bar1->o ; \
                       p_bar0->c = p_bar1->c ; \
                       p_bar0->h = p_bar1->h ; \
                       p_bar0->l = p_bar1->l ; \
                       p_bar0->v = p_bar1->v ;

#define BAR1_TO_BAR0   memcpy(p_bar0->TradingDay,p_bar1->TradingDay,9) ; \
                       memcpy(p_bar0->ca_btime,p_bar1->ca_btime,9) ; \
                       memcpy(p_bar0->ActionDay,p_bar1->ActionDay,9) ; \
                       p_bar0->o = p_bar1->o ; \
                       p_bar0->c = p_bar1->c ; \
                       p_bar0->h = p_bar1->h ; \
                       p_bar0->l = p_bar1->l ; \
                       p_bar0->v = p_bar1->v ;


#define NEW_BAR1       memcpy(p_bar1->TradingDay,tick->TradingDay,9) ; \
                       memcpy(p_bar1->ca_btime,tick->UpdateTime,9) ; \
                       memcpy(p_bar1->ActionDay,tick->ActionDay,9) ; \
                       p_bar1->o = tick->LastPrice ; \
                       p_bar1->c = tick->LastPrice ; \
                       p_bar1->h = tick->LastPrice ; \
                       p_bar1->l = tick->LastPrice ; \
                       p_bar1->vsum = tick->Volume ; \
                       p_bar1->v = 0 ;

#define UPDATE_BAR1    p_bar1->c = tick->LastPrice ; \
                       if (  p_bar1->h < tick->LastPrice ) { p_bar1->h = tick->LastPrice ; } \
                       if (  p_bar1->l > tick->LastPrice ) { p_bar1->l = tick->LastPrice ; } \
                       p_bar1->v    = tick->Volume - p_bar0->vsum ; \
                       p_bar1->vsum = tick->Volume ;

#define UPDATE_B1      b1->c = tick->LastPrice ; \
                       if ( b1->h < tick->LastPrice ) { b1->h = tick->LastPrice ; } \
                       if ( b1->l > tick->LastPrice ) { b1->l = tick->LastPrice ; } \
                       b1->vsum = tick->Volume ; \
                       b1->v = tick->Volume - b1->vold ;

#define NEW_B1         memcpy(b1->TradingDay,tick->TradingDay,9) ; \
                       memcpy(b1->cB,tick->UpdateTime,9) ; \
                       memcpy(b1->ActionDay,tick->ActionDay,9) ; \
                       b1->o = tick->LastPrice ; \
                       b1->c = tick->LastPrice ; \
                       b1->h = tick->LastPrice ; \
                       b1->l = tick->LastPrice ; \
                       b1->vold = b1->vsum; \
                       b1->vsum = tick->Volume ; \
                       b1->v = tick->Volume - b1->vold ; \
                       b1->sent = 0;


#define FIRST_TICK     if (  p_bar0->h < 0 ) { \
                           memcpy(p_bar0->TradingDay,tick->TradingDay,9) ; \
                           memcpy(p_bar0->ca_btime,tick->UpdateTime,9) ; \
                           memcpy(p_bar0->ActionDay,tick->ActionDay,9) ; \
                           p_bar0->o = tick->LastPrice ; \
                           p_bar0->c = tick->LastPrice ; \
                           p_bar0->h = tick->LastPrice ; \
                           p_bar0->l = tick->LastPrice ; \
                           p_bar0->v = tick->Volume ; \
                           memcpy(p_bar1->TradingDay,tick->TradingDay,9) ; \
                           memcpy(p_bar1->ca_btime,tick->UpdateTime,9) ; \
                           memcpy(p_bar1->ActionDay,tick->ActionDay,9) ; \
                           p_bar1->o = tick->LastPrice ; \
                           p_bar1->c = tick->LastPrice ; \
                           p_bar1->h = tick->LastPrice ; \
                           p_bar1->l = tick->LastPrice ; \
                           p_bar1->v = tick->Volume ; \
                           break ; \
                       }

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
  int   	   iType;  /* 不同的交易时间类型 */
  int          iSegNum; /* 多少个segment */
  Segment    aSgms[SGM_NUM] ;
};

struct TimeBlock {
  stTimeType TT[7] ;   // 有7种交易时间类型。参见 M_TimeType
public:
  TimeBlock();
private:
  int Init(stTimeType TT[]) ;
};

// ----- End ----------- 时间结构定义 ----------------------------------------

struct stBar {
  char    TradingDay[9];
  char    ActionDay[9];
  char    cB[9];   		//begin time BAR K柱 的开始时间
  char    cE[9];   		//end time
  int     iB ;          // 当前K柱 起始时间
  int     iE ;          // 当前K柱 结束时间
  int     iBidx;          // 此bar的起始点所在的 segment idx，
  int     iEidx;          // 此bar的结束点所在的 segment idx，
  double  h ;             // high
  double  o ;             // open
  double  c ;             // close
  double  l ;             // low
  int     v ;             // volume
  int     vsum ;          // keep volume sum
  int     vold ;          // 前一个K柱的结束时的量
  int     sent ;          // 有没有sent 或者写入  ==2 已写 ==1 已sent 但没有写 ==0 没发没写
};

struct stBarBo {
  stBar         bar0 ;
  stBar         bar1 ;
  stBar         *pbar0 ;
  stBar         *pbar1 ;
  /*
  int           iH;
  int           iM;
  int           iS;                 // 当前周期 秒 ，比如当前周期结束时间为09:16:32,这里的 iSec=32 在 NewBar里用到
  */
  char          curB[9];            //记录当前tick所在的段
  char          curE[9];
  int           curiX ;              //记录当前tick所在的段的下标
  int           iBidx;              // 此bar1的起始点所在的 segment idx，
  int           iEidx;              // 此bar1的结束点所在的 segment idx，
  int			fr ;                // 0:tick 1:2s, 60:1F ---- 周期 ： 以秒计
  int           iH;                 // 将周期转成 H M S, 比如：周期为 97 秒 转成： iPeriodH=0 ;
  int           iM;                 // iPeriodM = 1;
  int           iF;                 // iPeriodS = 37;  需要初始化！！
  char          c_save ;                    /* 's' 表示 save  'n' 表示 不需要save */
  int           i_bar_type ;                // 1 2 3 5 10 15   这个值可以用来计算 新来的tick是不是在同一个K
  char          c_bar_type ;                // S F H D W M J Y  BAR_SECOND BAR_MINUTE ...
  char          ca_table[128];                 /* database table name */
  int           iSegNum ;          // segment 数量
  Segment     *seg[100] ;        // segment array
} ;


struct BaBo {
  stBar         bar1 ;
  stBar         *b1 ;
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
  char          ca_table[128];                 /* database table name */
  int           iSegNum ;          // segment 数量
  Segment     *seg[100] ;        // segment array
public:
  BaBo(const char *pF, int fr, stTimeType  *pTimeType);
  int MakeTime(char *caTime, int T) ;
} ;

struct FuBo {
  std::shared_ptr<uBEE::DBPool> dbpool;
  uWS::Group<uWS::SERVER> * SG;
  char         InstrumentID[31];
  char         caFileName[1024];            // 用于记录"/home/rabbit/see/dat/rcv_dat/au/au1801",要组合 period
  int          iCurIdx ;                    // 用于记录收到tick时，是在哪个交易时间段内
  stTimeType  *pTimeType ;                  // TimeType
  BaBo        *pBaBo[50] ;                 // 1s 2s 3s ... 1f 2f 3f 5f ... 1h 5h ... 1y tick
public:
  FuBo(char *fuID, uBEE::TimeBlock *tmbo, uWS::Group<uWS::SERVER> *sg, const int period[],int len);
};


// -------------------- for sending -----------------
struct Kline {
  int iX;              // 索引号  0:1S  1:2S 2:3S 4:5S ...   100:tick=0;
  int iF;              //
  char cK[200];
};

struct barSG {
  char    InstrumentID[31];
  char    TradingDay[9];
  char    ActionDay[9];
  int     iN ;   // 有多少个KK是需要传送的。
  Kline   KK[50];
};

struct sTick{
  int     iX;              // 索引号  0:1S  1:2S 2:3S 4:5S ...   100:tick=0;
  int     iF;              // 周期  600 300 ...：:w
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

struct sKbar{
  int     iX;              // 索引号  0:1S  1:2S 2:3S 4:5S ...   100:tick=0;
  int     iF;              // 周期  600 300 ...：:w
  char    cB[9];           // begin time BAR K柱 的开始时间
  char    cE[9];           // end time
  double  h ;              // high
  double  o ;              // open
  double  c ;              // close
  double  l ;              // low
  int     v ;              // volume
  int     vsum ;           // keep volume sum
};

struct sData{
  char    InstrumentID[31];
  char    TradingDay[9];
  char    ActionDay[9];
  sTick   TT ;
  int     iN ;
  sKbar   KK[50];
};

static const int dLen = sizeof(sData) - 50 * sizeof(sKbar) ;
static const int tLen = sizeof(sTick) ;
static const int bLen = sizeof(sKbar) ;
static const int oLen = dLen + bLen ;

// ----- End ----------- 时间结构定义 ----------------------------------------

typedef struct {
  struct CThostFtdcDepthMarketDataField   rcv_tick;
  TThostFtdcDateType                      rcv_date;
  TThostFtdcTimeType                      rcv_time;
  TThostFtdcMillisecType                  rcv_msec;
  int                                     rcv_week;
} see_tick_t;

typedef struct  {
  char c_oc_flag ;    /* 'o':open   ;c':close */
  char ca_b[9] ;      /* ca_begin             */
  char ca_e[9] ;      /* ca_end               */

  char ca_15F_start[9] ;
  char ca_20F_start[9] ;
  char ca_30F_start[9] ;
  char ca_1H_start[9] ;
  char ca_2H_start[9] ;
  char ca_3H_start[9] ;
  char ca_4H_start[9] ;
  char ca_5H_start[9] ;
  char ca_1D_start[9] ;

  char ca_15F_end[9] ;
  char ca_20F_end[9] ;
  char ca_30F_end[9] ;
  char ca_1H_end[9] ;
  char ca_2H_end[9] ;
  char ca_3H_end[9] ;
  char ca_4H_end[9] ;
  char ca_5H_end[9] ;
  char ca_1D_end[9] ;
} see_segment_t ;   // 用于记录时间段，目前每个交易时间段为15分钟


typedef struct {
  char ca_begin[9];
  char ca_end[9];
} see_begin_end_t;

typedef struct {
  int             i_hour_type;  /* 不同的交易时间类型 */
  int             period;
  see_begin_end_t * pt_BEs[SEE_SGM_NUM];
} see_BE_t;

typedef struct  {
  int             i_hour_type;  /* 不同的交易时间类型 */
  see_segment_t  *pt_segments[SEE_SGM_NUM] ;
} see_hours_t ;     // 交易时间，用于记录一天的所有的交易时间段  每个期货品种有一个 see_fut_block_t，这个结构中会包含


// class TradingTime -----------------------

struct TradingTime {
  see_hours_t t_hours[10] ;   // 有10种交易时间类型。
public:
  TradingTime();
private:
  int Init(see_hours_t t_hours[]) ;
};
// class TradingTime  end  -----------------------


typedef struct {
//    char    InstrumentID[31];
  char    TradingDay[9];
  char    ActionDay[9];
  char    ca_btime[9];   //begin time BAR K柱 的开始时间
  char    ca_etime[9];  //end time
  int 	  i_btime ;
  int	  i_etime ;
  int     iBidx;
  int     iEidx;
  double  o ;             // open
  double  c ;             // close
  double  h ;             // high
  double  l ;             // low
  int     v ;             // volume
  int     vsum ;         // keep volume sum
} see_bar_t ;

typedef struct {
  see_bar_t           bar0 ;
  see_bar_t           bar1 ;
  char                c_save ;                    /* 's' 表示 save  'n' 表示 不需要save */
  int                 i_bar_type ;                // 1 2 3 5 10 15   这个值可以用来计算 新来的tick是不是在同一个K
  char                c_bar_type ;                // S F H D W M J Y  BAR_SECOND BAR_MINUTE ...
  char                ca_table[128];                 /* database table name */
  char                ca_home[512];              /* for txt ohlc saving */
  char                caFileName[2014];          // 文件名，用于保存 bar的内容。
} see_bar_block_t ;


typedef struct  {
  char        InstrumentID[31];
  char        ca_home[512];                   // 用于记录"/home/rabbit/see/dat/rcv_dat/au" 需要初始化
  char	      caTickFileName[1024];
  char	      caBarsFileName[1024];           // 用于记录"/home/rabbit/see/dat/rcv_dat/au/au1801",在使用时，要组合 period
  int         i_hour_type ;                   // 每个品种的交易时间类型不一样，有的到23:00结束，有的到 01:00
  char        c_oc_flag ;                     //  'o' 'c' 用于记录 收到的tick是不是在交易时间段内 每收到一个记一次
  int         i_sgm_idx ;                     // 用于记录收到tick时，是在哪个交易时间段内 每收到一个记一次
  int         iCurIdx ;                      // 用于记录收到tick时，是在哪个交易时间段内
  see_hours_t         *pt_hour ;                      // 每个品种的交易时间类型 不一样
  stTimeType         *pTimeType ;
  see_bar_block_t     bar_block[31] ;                 // 1s 2s 3s ... 1f 2f 3f 5f ... 1h 5h ... 1y tick
} see_fut_block_t ;


typedef struct {
  char                InstrumentID[31];
  char                TradingDay[9];
  char                ActionDay[9];
  char                UpdateTime[9];
  double              o ;             // open
  double              c ;             // close
  double              h ;             // high
  double              l ;             // low
  int                 v ;             // volume
} see_bar_save_t ;

//每个 合约一个 FuBlock
struct FuBlock {
  std::shared_ptr<uBEE::DBPool> dbpool;
  see_fut_block_t Block ;
public:
  FuBlock();
  int Init(see_fut_block_t * p_block, char * pc_future, see_hours_t t_hours[]);
private:
};

int string_split(char *s,char _cmd[SEE_SGM_NUM][20]);

int see_bars_index_next(int start_index) ;
int see_bars_index_back(int start_index, int n) ;
int see_bars_index_forword(int start_index, int n) ;


int see_date_comp(char * pca_first, char * pca_last) ;
int see_time_comp(char * pca_first, char * pca_last) ;
see_bar_t * see_create_bar(char * p_future_id, char c_period) ;

//int split_string(char *s,char _cmd[SEE_SGM_NUM][20]) ;
int see_time_to_int(char *f) ;
int see_handle_bars(see_fut_block_t *p_block, TICK *tick) ;
int see_send_bar(see_fut_block_t *p_block,char *pc_msg) ;
int see_save_bar(see_fut_block_t * p_block, TICK *tick, int period) ;
int see_save_bar_last(see_fut_block_t *p_block, int period, int i_another_day) ;
int is_weekend(char * pc_day) ;
int is_holiday(char * pc_day) ;
int is_notrade(see_fut_block_t * p_blick,char * time0) ;
int see_first_tick(see_fut_block_t *p_block, TICK *tick, see_bar_t *p_bar0, see_bar_t *p_bar1, int period) ;
int CalcBar(see_fut_block_t *p_block, TICK *tick, int period);
int is_mkt_open(see_fut_block_t *p_block, TICK *tick) ;
int is_same_k_bar(see_fut_block_t *p_block, see_bar_t *p_bar1, TICK *tick, int period);
int MakeTime(char *caT, int T) ;

int HandleTick(uBEE::FuBo *fubo, TICK *tick);
int MarkBar(uBEE::FuBo *fubo, TICK *tick,int period);
int SendBar(uBEE::FuBo *fubo, TICK *tick,int period);
int SaveBar(uBEE::FuBo *fubo, TICK *tick,int period);
int DealBar(uBEE::FuBo *fubo, TICK *tick,int period);

int Display(uBEE::FuBo *fubo, TICK *tick,int period,const char*msg);
int DispBar(uBEE::FuBo *fubo, TICK *tick,int period,const char*msg);


} //end namespace
#endif // UBEE_BARS_H

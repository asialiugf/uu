#ifndef UBEE_BITMAP_H
#define UBEE_BITMAP_H

#include <uWS/uWS.h>
#include <iostream>
#include <bitset>
#include <limits>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

namespace uBEE
{

//int IntToBit(uint64_t* d0, uint64_t* d1, int s, int s_pre, int bitx) ;
int LLongToBit(int64_t  dest[],   // destination
               int64_t  scur,    // source current
               int64_t  spre,    // source pre
               int     *bufx,  // buffer index
               int     *bitx,  // bit index
               int8_t  *hzeros,   // head zeros  xor 
               int8_t  *tzeros) ; // tail zeros  xor

// tick shard block
/*
struct sTick {                 // tick for send !!
  int     iType ;                 // type :  0:tick  1 2 3 ... for other ....
  char    InstrumentID[31];
  char    ActionDay[9];
  char    TradingDay[9];
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
 -----------
 '0' : same TradingDay[9]
  memcpy(uint64_t,TradingDay,8);
 '1' + '111111' + '111111'+ 'xxxxxxxxxx'
 -----------
 '0' : same UpdateTime[9]
  memcpy(uint64_t,UpdateTime,8);
 '1' + '111111' + '111111'+ 'xxxxxxxxxx'
 -----------

*/
struct TickShardBo {
  sTick     TickHead ;
  uint64_t  buff[10000] ;
  int       rate;
  int	    bufx;
  int       bitx;
  //sTick     Temp ;
  uint8_t          h_TradingDay;
  uint8_t          t_TradingDay;
  uint8_t          h_UpdateTime;          ///最后修改时间
  uint8_t          t_UpdateTime;          ///最后修改时间
  uint8_t          h_OpenPrice;              ///今开盘
  uint8_t          t_OpenPrice;              ///今开盘
  uint8_t          h_HighestPrice;           ///最高价
  uint8_t          t_HighestPrice;           ///最高价
  uint8_t          h_LowestPrice;            ///最低价
  uint8_t          t_LowestPrice;            ///最低价
  uint8_t          h_LastPrice;              ///最新价
  uint8_t          t_LastPrice;              ///最新价
  uint8_t          h_OpenInterest;           ///持仓量
  uint8_t          t_OpenInterest;           ///持仓量
  uint8_t          h_UpdateMillisec;         ///最后修改毫秒
  uint8_t          t_UpdateMillisec;         ///最后修改毫秒
  uint8_t          h_BidPrice1;              ///申买价一
  uint8_t          t_BidPrice1;              ///申买价一
  uint8_t          h_BidVolume1;             ///申买量一
  uint8_t          t_BidVolume1;             ///申买量一
  uint8_t          h_AskPrice1;              ///申卖价一
  uint8_t          t_AskPrice1;              ///申卖价一
  uint8_t          h_AskVolume1;             ///申卖量一
  uint8_t          t_AskVolume1;             ///申卖量一
  uint8_t          h_Volume;                 ///数量
  uint8_t          t_Volume;                 ///数量
};

} //namespace

#endif // UBEE_BITMAP_H

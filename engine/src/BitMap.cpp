#include <uWS/uWS.h>
#include <iostream>
#include <bitset>
#include <limits>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

namespace uBEE
{
int LLongToBit(int64_t     dest[],
               int64_t     scur,
               int64_t     spre,
               int         *bufx,
               int         *bitx,
               int8_t      *hzeros,
               int8_t      *tzeros)
{
  if(scur == spre) {  // same value
    // '0' -----
    *bitx+=1;
    if(*bitx==64) {
      *bufx +=1;
      *bitx = 0;
    }
    return 0;
  } else {  // not same value
    //  '0'  // 完全相同
    //  '10' + 'xxxxxxxxxx'      // headzeros相同  tailzeros 相同  //向外撑开
    //  '11' + '111111' + '??' + 'xxxxxxxxxx'
    //  '111111':6位表示 head_zeros,
    //  '???':表示 XOR 的长度， 是变长，如果 head_zeros<32, '??????' 是6位
    //  48>head_zeros>32  '?????'  是5位
    //  56>head_zeros>48  '????'   是4位
    //  64>head_zeros>56  '???'    是3位

    int64_t xordat = scur ^ spre ;
    std::bitset<64> data ;
    int8_t HeadZeros = __builtin_clzll(xordat);
    int8_t TailZeros = __builtin_ffsll(xordat);

    if(*hzeros <= HeadZeros && *tzeros <= TailZeros) {
      if((HeadZeros-*hzeros)<=5 && (TailZeros-*tzeros)<=5) {
        // '10'
        int64_t xorLen =(int64_t)(65-*hzeros-*tzeros) ;
        /*
        int64_t m1 = 1<<(63-*bitx);
        dest[0] = dest[0] | m1 ;
        if(*bitx>=62) {
          *bitx = *bitx-62 ;
          *bufx +=1;
        }
        */
        if(xorLen+2 <= 64-*bitx) {
          // d[0] is ok
          int64_t m1 = 1<<(63-*bitx);
          //xordat <<= (61-*bitx);
          dest[0] = dest[0] | m1 | xordat ;
          *bitx +=xorLen+2;
          if(*bitx==64) {
            *bitx = 0;
            *bufx+=1 ;
          }
          return 0;
        } else {
          if(*bitx<62) {
            int64_t m1 = 1<<(63-*bitx);
            dest[0] = dest[0] | m1 ;
            int64_t m2;
            if((*bitx+2-*hzeros) > 0) {
              m2 = xordat>>(*bitx-*hzeros) ;
            } else {
              m2 = xordat<<(*hzeros-*bitx-2) ;
            }
            dest[0] = dest[0] | m2 ;
            int64_t m3 = xordat << (*hzeros+64-*bitx-2) ;
            dest[1] = dest[1] | m3 ;
            *bitx = xorLen - (64-*bitx-2) ;
            *bufx+=1;
            return 0;

          } else if(*bitx==62) {
            dest[0] = dest[0] | 2 ;
            xordat <<= *hzeros ;
            dest[1] = dest[1] ^ xordat ;
            *bitx = xorLen;
            *bufx+=1 ;
          } else {
            dest[0] = dest[0] | 1 ;
            dest[2] = dest[2] | (xordat << (*hzeros-1)) ;
            if(xorLen <63) {
              *bitx = xorLen+1 ;
              *bufx +=1;
              return 0;
            } else if(xorLen ==63) {
              *bitx = 0;
              *bufx+=2;
              return 0;
            } else {
              dest[3] = dest[3] | (xordat << 63) ;
              *bitx=1;
              *bufx+=2;
               return 0;
            }
          }
        }
        return 0;
      }
      *hzeros = HeadZeros ;
      *tzeros = TailZeros ;
    } else {
      if(*hzeros > HeadZeros) {
        *hzeros = HeadZeros ;
      }
      if(*tzeros > TailZeros) {
        *tzeros=TailZeros;
      }
    }
    //'11' + '111111' + '????' + 'xxxxxxxxxx'
    //data[63-*bitx] = 1 ;
    int64_t xorLen =(int64_t)(65-*hzeros-*tzeros) ;
  }
}

} //namespace

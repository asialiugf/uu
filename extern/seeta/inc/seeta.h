/*
 * Copyright (C) AsiaLine
 * Copyright (C) kkk, Inc.
*/


#ifndef _SEE_TALIB_H_INCLUDED_
#define _SEE_TALIB_H_INCLUDED_

namespace uBEE
{

#define SEE_NULL -2147483648
#define ARRAY_SIZE 1000000
/*
#define PREFLAG if( H[i]==L[i] ) { \
                    if( C[i]>=H[i-1] && C[i]>L[i-1] ) { *preF=1;  }else{ \
                    if( C[i]<=L[i-1] && C[i]<H[i-1] ) { *preF=-1; }else{ \
                    if( C[i]< H[i-1] && C[i]>L[i-1] ) { *preF=0;  }}} \
                }else{ *preF=0; }

#define R_VALUE     if ( Hest-Lest != 0 ) { R[i] = (C[i]-Lest)/(Hest-Lest)*100 ; }else{ \
                    if (*preF==1){R[i]=100;}else if(*preF==-1){R[i]=0;}else if(*preF==0){R[i]=R[i-1];}}
*/
int SEE_MA(int start, int end, const double in[],  int N, double out[]) ;      /* out index and in index must be the same */
int SEE_EMA(int start, int end, const double in[],  int N, double out[]) ;
int SEE_SMA(int start, int end, const double in[],  int N, int M, double out[]) ;
int SEE_LMA(int start, int end, const double in[], char F, int N, int M, double out[]) ;

// ------------ 只输入一个序列 in[] --------------------
int SEE_RSV1(int           start,
             int           end,
             const double  in[],         /* outReal index and inReal index must be the same */
             double        *preH,        /* pre highest price */
             double        *preL,        /* pre lowest price */
             int           *preF,        /* 记录是涨停还是跌停 */
             int           N,            /* R:=(X-LLV(X,N))/(HHV(X,N)-LLV(X,N))*100; */
             double        out[]);       /* out[] 必须初始化为 SEE_NULL ; !!!!!!!!!!!!!!!!!!!!!   */

/*
 ------------ 输入三个序列 H[] L[] C[] --------------------
 如果 只计算一个值的RSV，那让 H L C 等于同一个输入序列即可。
*/
int SEE_RSV(int           start,
            int           end,
            const double  H[],          /* outReal index and inReal index must be the same */
            const double  L[],          /* outReal index and inReal index must be the same */
            const double  C[],          /* outReal index and inReal index must be the same */
            double        *preH,        /* pre highest price */
            double        *preL,        /* pre lowest price */
            int           *preF,        /* 记录是涨停还是跌停 */
            int           N,            /* R:=(CLOSE-LLV(LOW,N))/(HHV(HIGH,N)-LLV(LOW,N))*100; */
            double        R[]);         /* R[] 必须初始化为 SEE_NULL ; !!!!!!!!!!!!!!!!!!!!!   */

//--------------------RSV -- K ------------------------------------------------------
int SEE_K(int           start,
          int           end,
          const double  H[],          /* outReal index and inReal index must be the same */
          const double  L[],          /* outReal index and inReal index must be the same */
          const double  C[],          /* outReal index and inReal index must be the same */
          double        *preH,        /* pre highest price */
          double        *preL,        /* pre lowest price */
          int           *preF,        /* 记录是涨停还是跌停 */
          int           N,            /* _R:=(CLOSE-LLV(LOW,N))/(HHV(HIGH,N)-LLV(LOW,N))*100; */
          int           M1,           /*  K:=SMA(_R,M1,1);                                    */
          double        _R[],         /* _R[] 必须初始化为 SEE_NULL ; !!!!!!!!!!!!!!!!!!!!!   */
          double        K[]);         /*  K[] 必须初始化为 SEE_NULL ; !!!!!!!!!!!!!!!!!!!!!   */

//--------------------RSV -- K -- D ----------------------------------------------------
int SEE_KD(int           start,
           int           end,
           const double  H[],          /* outReal index and inReal index must be the same */
           const double  L[],          /* outReal index and inReal index must be the same */
           const double  C[],          /* outReal index and inReal index must be the same */
           double        *preH,        /* pre highest price */
           double        *preL,        /* pre lowest price */
           int           *preF,        /* 记录是涨停还是跌停 */
           int           N,            /* _R:=(CLOSE-LLV(LOW,N))/(HHV(HIGH,N)-LLV(LOW,N))*100; */
           int           M1,           /*  K:=SMA(_R,M1,1);                                    */
           int           M2,           /*  D:=SMA( K,M2,1);                                    */
           double        _R[],         /* _R[] 必须初始化为 SEE_NULL ; !!!!!!!!!!!!!!!!!!!!!   */
           double        K[],          /*  K[] 必须初始化为 SEE_NULL ; !!!!!!!!!!!!!!!!!!!!!   */
           double        D[]);         /*  D[] 必须初始化为 SEE_NULL ; !!!!!!!!!!!!!!!!!!!!!   */

//--------------------RSV -- K -- E ----------------------------------------------------
int SEE_KE(int           start,
           int           end,
           const double  H[],          /* outReal index and inReal index must be the same */
           const double  L[],          /* outReal index and inReal index must be the same */
           const double  C[],          /* outReal index and inReal index must be the same */
           double        *preH,        /* pre highest price */
           double        *preL,        /* pre lowest price */
           int           *preF,        /* 记录是涨停还是跌停 */
           int           N,            /* _R:=(CLOSE-LLV(LOW,N))/(HHV(HIGH,N)-LLV(LOW,N))*100; */
           int           M1,           /*  K:=SMA(_R,M1,1);                                    */
           int           N2,           /*  E:=EMA( K,N2);                                      */
           double        _R[],         /* _R[] 必须初始化为 SEE_NULL ; !!!!!!!!!!!!!!!!!!!!!   */
           double        K[],          /*  K[] 必须初始化为 SEE_NULL ; !!!!!!!!!!!!!!!!!!!!!   */
           double        E[]);         /*  E[] 必须初始化为 SEE_NULL ; !!!!!!!!!!!!!!!!!!!!!   */

//-------------------------RSV-E -- K -------------------------------------------------
int SEE_EK(int           start,
           int           end,
           const double  H[],          /* outReal index and inReal index must be the same */
           const double  L[],          /* outReal index and inReal index must be the same */
           const double  C[],          /* outReal index and inReal index must be the same */
           double        *preH,        /* pre highest price */
           double        *preL,        /* pre lowest price */
           int           *preF,        /* 记录是涨停还是跌停 */
           int           N,            /* _R:=(CLOSE-LLV(LOW,N))/(HHV(HIGH,N)-LLV(LOW,N))*100;  */
           int           N1,           /* _E:=EMA(_R,N1);                                       */
           int           M1,           /*  K:=SMA(_E,M1,1);                                     */
           double        _R[],         /* _R[] 必须初始化为 SEE_NULL ; !!!!!!!!!!!!!!!!!!!!!    */
           double        _E[],         /* _E[] _E:=EMA(_R,N1)                                   */
           double        K[]);          /*  K[]  K:=SMA(_E,M1,1)                                 */

//-------------------------RSV-E -- K -- D -------------------------------------------------
int SEE_EKD(int           start,
            int           end,
            const double  H[],          /* outReal index and inReal index must be the same */
            const double  L[],          /* outReal index and inReal index must be the same */
            const double  C[],          /* outReal index and inReal index must be the same */
            double        *preH,        /* pre highest price */
            double        *preL,        /* pre lowest price */
            int           *preF,        /* 记录是涨停还是跌停 */
            int           N,            /* _R:=(CLOSE-LLV(LOW,N))/(HHV(HIGH,N)-LLV(LOW,N))*100;  */
            int           N1,           /* _E:=EMA(_R,N1);                                       */
            int           M1,           /*  K:=SMA(_E,M1,1);                                     */
            int           M2,           /*  D:=SMA( K,M2,1);                                     */
            double        _R[],         /* _R[] 必须初始化为 SEE_NULL ; !!!!!!!!!!!!!!!!!!!!!    */
            double        _E[],         /* _E[]                                                  */
            double        K[],          /*  K[]                                                  */
            double        D[]);         /*  D[]                                                  */

//-------------------------RSV-E -- K -- E -------------------------------------------------
int SEE_EKE(int           start,
            int           end,
            const double  H[],          /* outReal index and inReal index must be the same */
            const double  L[],          /* outReal index and inReal index must be the same */
            const double  C[],          /* outReal index and inReal index must be the same */
            double        *preH,        /* pre highest price */
            double        *preL,        /* pre lowest price */
            int           *preF,        /* 记录是涨停还是跌停 */
            int           N,            /* _R:=(CLOSE-LLV(LOW,N))/(HHV(HIGH,N)-LLV(LOW,N))*100;  */
            int           N1,           /* _E:=EMA(_R,N1);                                       */
            int           M1,           /*  K:=SMA(_E,M1,1);                                     */
            int           N2,           /*  E:=EMA( K,N2);                                       */
            double        _R[],         /* _R[] 必须初始化为 SEE_NULL ; !!!!!!!!!!!!!!!!!!!!!    */
            double        _E[],         /* _E[]                                                  */
            double        K[],          /*  K[]                                                  */
            double        E[]);         /*  E[]                                                  */


//-------------------------RSV -- K -- D -- J  -------------------------------------------------
int SEE_KDJ(int           start,
            int           end,
            const double  H[],          /* outReal index and inReal index must be the same */
            const double  L[],          /* outReal index and inReal index must be the same */
            const double  C[],          /* outReal index and inReal index must be the same */
            double        *preH,        /* pre highest price */
            double        *preL,        /* pre lowest price */
            int           *preF,        /* 记录是涨停还是跌停 */
            int           N,            /* R:=(CLOSE-LLV(LOW,N))/(HHV(HIGH,N)-LLV(LOW,N))*100; */
            int           M1,           /* K:=SMA(R,M1,1);                                     */
            int           M2,           /* D:=SMA(K,M2,1);                                     */
            double        R[],          /* R:=(CLOSE-LLV(LOW,N))/(HHV(HIGH,N)-LLV(LOW,N))*100; */
            double        K[],          /*    */
            double        D[],
            double        J[]) ;

int SEE_KDJ1(int           start,
             int           end,
             const double  H[],          /* outReal index and inReal index must be the same */
             const double  L[],          /* outReal index and inReal index must be the same */
             const double  C[],          /* outReal index and inReal index must be the same */
             double        *preH,        /* pre highest price */
             double        *preL,        /* pre lowest price */
             int           *preF,        /* 记录是涨停还是跌停 */
             int           N,            /* R:=(CLOSE-LLV(LOW,N))/(HHV(HIGH,N)-LLV(LOW,N))*100; */
             int           M1,           /* K:=SMA(R,M1,1);                                     */
             int           M2,           /* D:=SMA(K,M2,1);                                     */
             double        R[],          /* R:=(CLOSE-LLV(LOW,N))/(HHV(HIGH,N)-LLV(LOW,N))*100; */
             double        K[],          /*    */
             double        D[],
             double        J[]) ;

int SEE_MACD(int          start,
             int          end,
             const double in[],
             int          L,
             int          S,
             int          M,
             double       dif[],
             double       dea[],
             double       macd[]) ;

/*
LC:=REF(CLOSE,1);
RSI1:SMA(MAX(CLOSE-LC,0),N1,1)/SMA(ABS(CLOSE-LC),N1,1)*100;
RSI2:SMA(MAX(CLOSE-LC,0),N2,1)/SMA(ABS(CLOSE-LC),N2,1)*100;
RSI3:SMA(MAX(CLOSE-LC,0),N3,1)/SMA(ABS(CLOSE-LC),N3,1)*100;
*/
int SEE_RSI(int          start,
            int          end,
            const double in[],
            int          N,
            double       _max[],       //中间变量 要保留
            double       _abs[],       //中间变量 要保留
            double       _sma1[],       //中间变量 要保留
            double       _sma2[],       //中间变量 要保留
            double       rsi[]) ;      /* rsi 初始化为 SEE_NULL                           */

int SEE_BOLL(int           start,
             int           end,
             const double  in[],        /* outReal index and inReal index must be the same */
             int           N,           /* From 0 to ARRAY_SIZE                                */
             double        _MA[],       /* 保留中间计算和值 */
             double        E[],
             double        U[],
             double        D[]);


} // namespace

#endif /* _SEE_TALIB_H_INCLUDED_ */

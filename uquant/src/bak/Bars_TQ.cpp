#include "Bars.h"
#include "ErrLog.h"
#include "FuList.h"
#include "File.h"
#include "Global.h"
#include <cjson/cJSON.h>
#include <iostream>
#include <string.h>
#include <map>
#include <float.h>
#include <limits.h>


namespace uBEE
{

// ---------------------------------------------------------
TimeBlock::TimeBlock()
{
  cJSON   *jRoot ;
  cJSON   *jTime ;
  cJSON   *jTemp ;
  char ca_h[3];
  char ca_m[3];
  int  ih;
  int  im;
  int    i, n ;
  const std::map<int,std::string> *MP ;
  MP = &M_TimeType ;

  for(auto it = (*MP).begin(); it != (*MP).end(); ++it) {
    TT[it->first].iType = it->first ;
    for(i=0; i<SGM_NUM; i++) {
      TT[it->first].aSgms[i].iB = -1;
      TT[it->first].aSgms[i].iE = -1;
      TT[it->first].aSgms[i].iI = -1;
    }

    jRoot = cJSON_Parse(it->second.c_str());
    jTime = cJSON_GetObjectItem(jRoot, "time");
    n = cJSON_GetArraySize(jTime);

    TT[it->first].iSegNum = n;
    uBEE::ErrLog(1000," enter into Klines()!",1,0,0);
    std::cout << TT[it->first].iSegNum << "pppppppppppppppppppppp" << std::endl;
    for(i=0; i<n; i++) {
      jTemp = cJSON_GetArrayItem(jTime,i);

      std::cout << jTemp->valuestring << std::endl;
      see_memzero(TT[it->first].aSgms[i].cB,9);
      see_memzero(TT[it->first].aSgms[i].cE,9);

      memcpy(TT[it->first].aSgms[i].cB,jTemp->valuestring,5);
      memcpy(TT[it->first].aSgms[i].cB+5,":00",3);
      memcpy(TT[it->first].aSgms[i].cE,jTemp->valuestring+6,5);
      memcpy(TT[it->first].aSgms[i].cE+5,":00",3);
      std::cout << "cE: " << TT[it->first].aSgms[i].cE << std::endl;

      see_memzero(ca_h,3);
      see_memzero(ca_m,3);
      memcpy(ca_h,TT[it->first].aSgms[i].cB,  2);
      memcpy(ca_m,TT[it->first].aSgms[i].cB+3,2);
      ih = atoi(ca_h);
      im = atoi(ca_m);
      TT[it->first].aSgms[i].iB = ih*3600 + im*60 ;

      see_memzero(ca_h,3);
      see_memzero(ca_m,3);
      memcpy(ca_h,TT[it->first].aSgms[i].cE,  2);
      memcpy(ca_m,TT[it->first].aSgms[i].cE+3,2);
      ih = atoi(ca_h);
      im = atoi(ca_m);
      TT[it->first].aSgms[i].iE = ih*3600 + im*60 ;

      if(i == 0) {
        TT[it->first].aSgms[i].iI = 0;
      } else {
        if(TT[it->first].aSgms[i].iB >= TT[it->first].aSgms[i-1].iE) {
          TT[it->first].aSgms[i].iI =
            TT[it->first].aSgms[i].iB - TT[it->first].aSgms[i-1].iE;
        } else {
          TT[it->first].aSgms[i].iI =
            TT[it->first].aSgms[i].iB - TT[it->first].aSgms[i-1].iE + 86400;
        }
      }

    }
    //std::cout << "mmmm " << jTemp->valuestring << std::endl;
  }
  cJSON_Delete(jRoot);
}


int TimeBlock::Init(stTimeType TT[])
{

}


int BaBo::MakeTime(char *caT, int T)
{
  int h,m,s;
  char cTmp[9];
  h = T / 3600;
  m =(T - h*3600) / 60;
  s = T % 60;
  see_memzero(cTmp,9);
  sprintf(cTmp,"%02d:%02d:%02d",h,m,s);
  memcpy(caT, cTmp, 9);
  return 0;
}

/*
  // ????????? bar block(BaBo) !!! ????????? future block (FuBo) ??? 50??? BaBo ;
  // ??????future?????????????????????????????? ?????? FuBo ??????????????????pTimeType
  // ??????frequency + pTimeType??? ????????? BaBo??? ???stSegment     *seg[100] ;???
  //
struct stTimeType {
  int          iType;
  int          iSegNum;
  stSegment    aSgms[SGM_NUM] ;
};
*/
BaBo::BaBo(const char * pF, int iFr, stTimeType  *pTimeType)
{
  char cB[9];
  char cE[9];

  char cT[9];
  int  iT ;
  int  idxT ;

  char cTmp[9];
  int  iB ;
  int  iE ;
  int  iI ;
  int  mark ;

  if(T________) {
    sprintf(ca_errmsg,"--------------------------------------in BaBo: pF: %s  iFr:%d ",pF,iFr) ;
    uBEE::ErrLog(1000,ca_errmsg,1,0,0) ;
  }
  // ???????????? ----------------
  iF = iFr ;
  iH = iFr/3600;
  iM = (iFr-iH*3600)/60;
  iS = iFr%60;
  memcpy(cF,pF,strlen(pF));

  for(int i=0; i<100; i++) {
    seg[i] = nullptr;
  }

  int last = 0;   // ??????bar???seg??????????????????seg?????????????????????
  int idx  = 0;
  for(int i=0; i<pTimeType->iSegNum; i++) {
    iB = pTimeType->aSgms[i].iB ;
    iE = pTimeType->aSgms[i].iE ;

    if(T________) {
      sprintf(ca_errmsg,"------ i:%d iSegNum:%d iB:%d  iE:%d last:%d ",i,pTimeType->iSegNum,iB,iE,last) ;
      uBEE::ErrLog(1000,ca_errmsg,1,0,0) ;
      sprintf(ca_errmsg,"------ pTimeType->aSgms[i].cB:%s .cE:%s",pTimeType->aSgms[i].cB,pTimeType->aSgms[i].cE) ;
      uBEE::ErrLog(1000,ca_errmsg,1,0,0) ;
    }

    if(last>0) {
      if(iB + last <= iE) {
        seg[idx] = (stSegment *)malloc(sizeof(stSegment)) ;

        seg[idx]->iB = iB ;
        seg[idx]->iE = iB + last ;

        mark ++ ;
        seg[idx]->mark = mark;

        MakeTime(seg[idx]->cB,seg[idx]->iB) ;
        MakeTime(seg[idx]->cE,seg[idx]->iE) ;

        if(T________) {
          sprintf(ca_errmsg,"2222--- idx:%d mark:%d seg[idx]->cB:%s seg[idx]->cE:%s",idx,mark,seg[idx]->cB,seg[idx]->cE) ;
          uBEE::ErrLog(1000,ca_errmsg,1,0,0) ;
        }

        memcpy(seg[idx]->barB,cT , 9);
        seg[idx]->bariB = iT ;
        seg[idx]->barBx = idxT ;
        for(int j =0; j<mark; j++) {    // ???A???
          int k = idx-j ;
          memcpy(seg[k]->barE,seg[idx]->cE , 9);
          seg[k]->bariE = seg[idx]->iE ;
          seg[k]->barEx = idx ;

          if(T________) {
            sprintf(ca_errmsg,"3333--- idx:%d mark:%d seg[idx]->barB:%s seg[idx]->barE:%s",idx,seg[k]->mark,seg[k]->barB,seg[k]->barE) ;
            uBEE::ErrLog(1000,ca_errmsg,1,0,0) ;
          }
        }

        mark = 0;
        idx++ ;

      } else if(iB + last > iE) {
        seg[idx] = (stSegment *)malloc(sizeof(stSegment)) ;

        seg[idx]->iB = iB ;
        seg[idx]->iE = iE ;

        mark ++;
        seg[idx]->mark = mark ;

        MakeTime(seg[idx]->cB,seg[idx]->iB) ;
        MakeTime(seg[idx]->cE,seg[idx]->iE) ;

        if(T________) {
          sprintf(ca_errmsg,"4444--- idx:%d mark:%d seg[idx]->cB:%s seg[idx]->cE:%s",idx,mark,seg[idx]->cB,seg[idx]->cE) ;
          uBEE::ErrLog(1000,ca_errmsg,1,0,0) ;
        }

        memcpy(seg[idx]->barB,cT , 9);
        seg[idx]->bariB = iT ;
        seg[idx]->barBx = idxT ;
        if(i == pTimeType->iSegNum-1) {
          for(int j =0; j<mark; j++) {    // ???A???
            int k = idx-j ;
            memcpy(seg[k]->barE,seg[idx]->cE , 9);
            seg[k]->bariE = seg[idx]->iE ;
            seg[k]->barEx = idx ;

            if(T________) {
              sprintf(ca_errmsg,"5555--- idx:%d mark:%d segBE:%s--%s  barBE:%s--%s",idx,
                      seg[k]->mark,
                      seg[k]->cB,  seg[k]->cE,
                      seg[k]->barB,seg[k]->barE) ;
              uBEE::ErrLog(1000,ca_errmsg,1,0,0) ;
            }
          }
          idx++;
          break;
        }

        last =  last + iB - iE ;
        idx++;
        continue ;
      }
    }

    iB = iB + last ;
    int len =  iE - iB ;
    int num = len / iF ;
    int mo  = len % iF ;

    if(len ==0) {
      last = 0;
      continue ;
    }

    if(mo>0) {
      last = iF-mo;
    } else {
      last = 0 ;
    }

    if(T________) {
      sprintf(ca_errmsg,"6666--- idx:%d iB:%d len:%d num:%d mo:%d last:%d",idx,iB,len,num,mo,last) ;
      uBEE::ErrLog(1000,ca_errmsg,1,0,0) ;
    }

    if(num > 0) {
      seg[idx] = (stSegment *)malloc(sizeof(stSegment)) ;
      seg[idx]->iB = iB ;
      seg[idx]->iE = iE-mo ;
      mark =0;
      seg[idx]->mark =0 ;

      MakeTime(seg[idx]->cB,seg[idx]->iB) ;
      MakeTime(seg[idx]->cE,seg[idx]->iE) ;

      if(T________) {
        sprintf(ca_errmsg,"7777--- idx:%d mark:%d seg B:%s seg E:%s",idx,mark,seg[idx]->cB,seg[idx]->cE) ;
        uBEE::ErrLog(1000,ca_errmsg,1,0,0) ;
      }


      idx++;
    }

    if(mo > 0) {
      seg[idx] = (stSegment *)malloc(sizeof(stSegment)) ;
      seg[idx]->iB = iE-mo ;
      seg[idx]->iE = iE ;
      mark = 1;
      seg[idx]->mark = mark;

      MakeTime(seg[idx]->cB,seg[idx]->iB) ;
      MakeTime(seg[idx]->cE,seg[idx]->iE) ;

      if(T________) {
        sprintf(ca_errmsg,"8888--- idx:%d mark:%d mo:%d seg B:%s seg E:%s",idx,mark,mo,seg[idx]->cB,seg[idx]->cE) ;
        uBEE::ErrLog(1000,ca_errmsg,1,0,0) ;
      }

      memcpy(seg[idx]->barB,seg[idx]->cB , 9);
      seg[idx]->bariB = seg[idx]->iB ;
      seg[idx]->barBx = idx ;
      if(i == pTimeType->iSegNum-1) {
        memcpy(seg[idx]->barE,seg[idx]->cE,9);
        seg[idx]->bariE = seg[idx]->iE ;
        seg[idx]->barEx = idx ;
        if(T________) {
          sprintf(ca_errmsg,"9999--- idx:%d mark:%d seg[idx]->barB:%s seg[idx]->barE:%s",idx,seg[idx]->mark,seg[idx]->barB,seg[idx]->barE) ;
          uBEE::ErrLog(1000,ca_errmsg,1,0,0) ;
        }
      }
      memcpy(cT,seg[idx]->cB , 9);
      iT = seg[idx]->iB ;
      idxT = idx ;
      idx++ ;
    }
  } // --- end for

  iSegNum = idx;
  if(T________) {
    sprintf(ca_errmsg,"-------------------iSegNum:%d",iSegNum) ;
    uBEE::ErrLog(1000,ca_errmsg,1,0,0) ;
  }

  // ------- ????????????????????? seg. -- ????????????????????????Kbar????????? ------
  curiX = iSegNum ;
  seg[idx] = (stSegment *)malloc(sizeof(stSegment)) ;
  seg[idx]->sn = 0 ;

  see_memzero(seg[idx]->cB,9);
  see_memzero(seg[idx]->cE,9);
  see_memzero(seg[idx]->barB,9);
  see_memzero(seg[idx]->barE,9);
  seg[idx]->iB =0;
  seg[idx]->iE =0;
  seg[idx]->bariB =0;
  seg[idx]->bariE =0;
  seg[idx]->mark = -1;
  see_memzero(curB,9);
  see_memzero(curE,9);
  // ---------------------------- ?????????  bar0 bar1 ----------------

  for(int i=0; i<iSegNum; i++) {
    seg[i]->sn = 0 ;
    if(i < iSegNum-1) {
      if(memcmp(seg[i]->cE,seg[i+1]->cB,8) != 0) {
        seg[i]->sn = 1 ;
        if(memcmp(seg[i]->cE,"24:00:00",8)==0 && memcmp(seg[i+1]->cB,"00:00:00",8)==0) {
          seg[i]->sn = 0 ;
        }
      }
    } else {
      seg[i]->sn = 1 ;
    }
  }
  // -----------------------------------------------------------------
  /*
  if(seg[0]->mark>0) {
    memcpy(curB,seg[0]->cB,9);
    memcpy(curE,seg[0]->cE,9);
    curiB = seg[0]->iB;
    curiE = seg[0]->iE;
  } else {
    memcpy(curB,seg[0]->cB,9);
    MakeTime(curE,(seg[0]->iB + iF)) ;
    curiB = seg[0]->iB;
    curiE = seg[0]->iB + iF;
  }
  */

  //pbar0 = &bar0 ;
  pbar1 = &bar1 ;
  //b0 = pbar0;
  b1 = pbar1;

  stBar tmpBar ;
  see_memzero(tmpBar.TradingDay,9);
  see_memzero(tmpBar.ActionDay,9);
  see_memzero(tmpBar.cB,9);
  see_memzero(tmpBar.cE,9);
  /*
  if(seg[0]->mark>0) {
    memcpy(tmpBar.cB,seg[0]->barB,9);
    memcpy(tmpBar.cE,seg[0]->barE,9);
    //tmpBar.iB = -1;
    //tmpBar.iE = -1;
  } else {
    memcpy(tmpBar.cB,curB,9);
    memcpy(tmpBar.cE,curE,9);
    //tmpBar.iB = -1;
    //tmpBar.iE = -1;
  }
  */
  //tmpBar.iBidx = -1;
  //tmpBar.iEidx = -1;
  tmpBar.h = DBL_MIN;
  tmpBar.o = SEE_NULL;
  tmpBar.c = SEE_NULL;
  tmpBar.l = DBL_MAX;
  tmpBar.v = 0;
  tmpBar.vsum = 0;   // ??????????????????bar ??? v ??? ????????????????????????
  tmpBar.sent = 0;
  // ----------????????? #define UPDATE_B1 ?????????

  //memcpy((char *)pbar0,&tmpBar,sizeof(stBar)) ;
  memcpy((char *)pbar1,&tmpBar,sizeof(stBar)) ;

  //------ ?????????bar block???????????? curB curE


  if(T________) {
    sprintf(ca_errmsg,"--------------------------------------out BaBo\n\n") ;
    uBEE::ErrLog(1000,ca_errmsg,1,0,0) ;
  }
}

/*
  Future Block !
*/

FuBo::FuBo(char *caFuture, uBEE::TimeBlock *tmbo, const int aFr[],int len)
{
  if(T________) {
    sprintf(ca_errmsg,"\nFuBo::FuBo() :enter!!--------into FuBo:caFuture:%s len:%d",caFuture,len) ;
    uBEE::ErrLog(1000,ca_errmsg,1,0,0) ;
  }

  see_memzero(InstrumentID,31);
  memcpy(InstrumentID,caFuture,strlen(caFuture));
  iCurIdx = -1;

  // charmi-- ??? ag1803 ????????? "ag"
  char fn[3] ;
  see_memzero(fn,3);
  memcpy(fn,caFuture,2) ;
  if(fn[1] <= '9' && fn[1] >= '0') {
    fn[1] = '\0';
  }

  // ????????? pTimeType [stTimeType  *pTimeType]
  std::map<std::string,int>::const_iterator it;
  it = M_FuTime.find(fn);
  if(it==M_FuTime.end()) {
    sprintf(ca_errmsg,"FuBo::FuBo() :M_FuTime error:not defined: %s %s",InstrumentID,fn) ;
    uBEE::ErrLog(1000,ca_errmsg,1,0,0) ;
  } else {
    sprintf(ca_errmsg,"FuBo::FuBo(): future:%s %s timetype:%d, TimeType:",InstrumentID,fn,it->second) ;
    uBEE::ErrLog(1000,ca_errmsg,1,0,0) ;
    pTimeType = &tmbo->TT[it->second] ;
  }

  // -- ????????? BaBo[50] ; ----------------------------------------
  /*
   ???????????????????????????????????? frequency : fr
   fr= 1 ???????????????1???
   fr= 60 ???????????????60???

   ?????????????????????30??????
   0  1 2 3 4  5  6  7  8  9  10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29
   1S 2 3 5 10 15 20 30 1F 2F 3F 5F 10 15 20 30 1H 2H 3H 4H 5H 6H 8H 10 12 1D 1W 1M 1J 1Y
   ?????????????????????????????????20??????
   pBaBo[50] :
   pBaBo[0-29]
   pBaBo[30-49]
   ------------------------
   0  1 2 3 4  5  6  7  8  9   10  11  12  13  14   15   16   17   18    19    20    21 22 23 24 25 26 27 28 29
   1S 2 3 5 10 15 20 30 1F 2F  3F  5F  10  15  20   30   1H   2H   3H    4H    5H    6H 8H 10 12 1D 1W 1M 1J 1Y
   1, 2,3,5,10,15,20,30,60,120,180,300,600,900,1200,1800,3600,7200,10800,14400,18000, 0, 0, 0, 0, 0, 0, 0, 0, 0};
  */
  // ??????????????? ?????????---------------------------------------------------------
  // ?????? FuBo --> future block ??? 50?????????????????? ?????????????????? BaBo ( bar block )
  int i = 0;
  for(i=0; i<50; i++) {
    pBaBo[i] = nullptr;
  }

  i = 0;
  for(auto it = M_FF.begin(); it != M_FF.end(); ++it) {
    if(it->second !=0) {
      sprintf(ca_errmsg,"FuBo::FuBo(): %s %d",it->first.c_str(),it->second) ;
      uBEE::ErrLog(1000,ca_errmsg,1,0,0) ;

      pBaBo[i] = new BaBo(it->first.c_str()+4,it->second, pTimeType); // +4:????????????100_ 101_
    }
    i++;
  }

  // ????????????????????? -----------------------------------------------------
  // pBaBo[] ??? 30--50????????????????????? ------
  // ???????????????????????????????????????20???  ------
  // ??????????????????????????????????????????????????? ------
  int m = 30;
  for(i=0; i<len; i++) {
    if(aFr[i]==0) {
      continue ;
    }
    sprintf(ca_errmsg,"FuBo::FuBo(): custom frequency: i:%d  frequency:%d",i,aFr[i]) ;
    uBEE::ErrLog(1000,ca_errmsg,1,0,0) ;

    pBaBo[m] = new BaBo("mm",aFr[i], pTimeType);
    m++;

    if(m >=50) {
      break;
    }
  }
// ?????? ???????????????  ---------------------------------------------------
  if(T________) {
    sprintf(ca_errmsg,"FuBo::FuBo():---------------------out FuBo\n\n\n") ;
    uBEE::ErrLog(1000,ca_errmsg,1,0,0) ;
  }
}

// ---------------------------------------------------------
/*
  ?????????period???????????? fubo->aBarBo[]???????????????
*/
//int DealBar(see_fut_block_t *p_block, TICK *tick,int period)
int DealBar(uBEE::FuBo *fubo, TICK *tick,int period)
{
  //stBar       *p_bar0;
  stBar       *p_bar1;
  //stBar       *b0;
  stBar       *b1;
  stBar       *bt;

  //b0 = fubo->pBaBo[period]->b0 ;
  b1 = fubo->pBaBo[period]->b1 ;

  //p_bar0 =  &fubo->pBaBo[period]->bar0;
  p_bar1 =  &fubo->pBaBo[period]->bar1;

  char * curB = fubo->pBaBo[period]->curB ;
  char * curE = fubo->pBaBo[period]->curE ;
  int &curiB = fubo->pBaBo[period]->curiB ;
  int &curiE = fubo->pBaBo[period]->curiE ;
  int &curiX = fubo->pBaBo[period]->curiX ;

  char * barB = b1->cB ;
  char * barE = b1->cE ;
  int x = 0;
  int i = 0;

  BaBo * babo = fubo->pBaBo[period] ;

//  char * segB = fubo->pBaBo[period]->seg[1]->cB ;
//  char * segE = fubo->pBaBo[period]->seg[1]->cE ;

#define SEGB fubo->pBaBo[period]->seg[curiX]->cB
#define SEGE fubo->pBaBo[period]->seg[curiX]->cE
#define MARK babo->seg[curiX]->mark
#define SN babo->seg[curiX]->sn

  char * tik = tick->UpdateTime ;

  //int sent ;
  int mark ;

  int fr = fubo->pBaBo[period]->iF ;

  Display(fubo,tick,period,"eeee:enter:");

  // ??????tick?????? curB---curE????????????UPDATE???
  if(memcmp(tick->UpdateTime,curB,8)>=0 &&
     memcmp(tick->UpdateTime,curE,8)<0) { // ???????????? curB curE ??????segment???
    UPDATE_B1;
    Display(fubo,tick,period,"eeee:uuu---");
    return 0;
  }

  if(memcmp(tick->UpdateTime,curE,8)==0) {
    if(SN==1 && memcmp(tik,SEGE,8)==0) {
      if(tick->UpdateMillisec < 500) {
        UPDATE_B1;
        Display(fubo,tick,period,"eeee:uuu---");
        return 0;
      }
    }
  }

  //xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
mark0:
  if(MARK == 0) {

    if(memcmp(tik,SEGE,8) <0) {
      // ???A??????barE==tick <segE  ???   ==>  e         s         {????????????bar}
      if(memcmp(barE,tik,8)==0) {
        if(tick->UpdateMillisec < 500) {
          return 0;
        }
        NEW_B1;
        curiB = curiB+fr;
        curiE = curiE+fr;
        memcpy(curB,curE,9) ;
        uBEE::MakeTime(curE,curiE);
        memcpy(b1->cB,curB,9);
        memcpy(b1->cE,curE,9);

        Display(fubo,tick,period,"eeee:000---");
        return 0;
      } //------- A

      // ???B??????barE <tick <segE  ???   ==>  f         s or 2    {??????????????????bar}
      //  1??? ?????????????????????????????? SendBar()???????????????K???
      //  2??? ?????? tick?????????????????????????????? memcmp(tik,barE,8)==0???????????????tick ????????????K+1?????????????????????????????? 3 4.
      //  3??? NEW_B1; ????????????????????????tick?????? K+1??? ????????? ????????????????????????K+1??????????????????tick
      //  4:  ?????????????????? SendBar() SaveBar() ---> goto mark0:
      // ???B??????barE <tick <segE  ???   ==>  f         s or 2    {??????????????????bar}
      if(memcmp(barE,tik,8)<0) {
        do {
          curiB = curiE ;
          curiE += fr ;
          memcpy(curB,curE,9) ;
          uBEE::MakeTime(curE,curiE);
        } while(memcmp(tik,curE,8)>0);

        NEW_B1;
        memcpy(barB,curB,9);
        memcpy(barE,curE,9);

        Display(fubo,tick,period,"eeee:011---");

        if(memcmp(tik,barE,8)==0) {
          Display(fubo,tick,period,"eeee:goo---0");
          SendBar(fubo,tick,period);
          SaveBar(fubo,tick,period);
          goto  mark0;
        }
        return 0;
      } //------- B
    }

    // -------------------------------------------------------------------------------
    // ???C??????barE==tick==segE  ???   ==>  a         s         {????????????bar}
    // ???D??????barE <tick==segE  ???   ==>  a         s +  2    {??????????????????bar}
    if(memcmp(tik,SEGE,8)==0) {
      if(memcmp(barE,tik,8)==0) {  //----C
        goto aaa;
      }
      if(memcmp(barE,tik,8)<0) {   //----D   ???????????? C
        NEW_B1;
        curiB = babo->seg[curiX]->iE-fr ;
        curiE = babo->seg[curiX]->iE ;
        uBEE::MakeTime(curB,curiB);
        memcpy(curE,SEGE,9) ;

        memcpy(barB,curB,9);
        memcpy(barE,curE,9);

        Display(fubo,tick,period,"eeee:goo---0");
        SendBar(fubo,tick,period);
        SaveBar(fubo,tick,period);
        goto  aaa;
        //goto  mark0;
      }
      return 0;
    } //------- C D

    // -------------------------------------------------------------------------------
    //???E??????barE<=segE <tick  ???   ==>  b         s
    //???F??????tick <barE<=segE  ???   ==>  b + 0?    s         {????????????bar}   1???0?????????
    //if((memcmp(barE,SEGE,8)<=0 && memcmp(SEGE,tik,8)<0) ||
    // ?????? MARK==0 ????????? barE<=segE??????????????????????????????????????????
    if(memcmp(SEGE,tik,8)<0 || memcmp(tik,barE,8)<0) {
      Display(fubo,tick,period,"eeee:1:E,F---");
      goto bbb;
    } //------- E F
  }

  ////xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
mark1:
  if(MARK > 0) {
    //???A??????segE==tick <barE    ???  ==   c
    //???1??????tick==segE> barE    ???  ==>  c                          ??????seg1
    if(memcmp(SEGE,tik,8)==0 &&(memcmp(tik,barE,8)<0 || memcmp(SEGE,barE,8)>0)) {
      Display(fubo,tick,period,"eeee:2:A,1---");

      UPDATE_B1;
      curiX++;
      curiB = babo->seg[curiX]->iB ;
      curiE = babo->seg[curiX]->iE ;
      memcpy(curB,babo->seg[curiX]->cB,9);
      memcpy(curE,babo->seg[curiX]->cE,9);

      return 0;
    } //------- A 1

    //???B??????segE< tick <barE    ???  ==>  d
    //???2??????tick> segE >barE    ???  ==>  d                          ??????seg2
    //???3??????segE> barE >tick    ???  ==>  d                          ??????seg3 4 5
    if((memcmp(SEGE,tik,8)<0 && memcmp(tik,barE,8)<0) ||
       (memcmp(tik,SEGE,8)>0 && memcmp(SEGE,barE,8)>0) ||
       (memcmp(SEGE,barE,8)>0 && memcmp(barE,tik,8)>0)) {
      Display(fubo,tick,period,"eeee:2:B,2,3---");
      int i = curiX;
      do {
        i++;
      } while((memcmp(barE,babo->seg[i]->barE,8)==0) &&
              (memcmp(tik,babo->seg[i]->cB,8)<0 || memcmp(tik,babo->seg[i]->cE,8)>0));
      if(memcmp(barE,babo->seg[i]->barE,8)!=0) {
        return 0;
      }

      UPDATE_B1;
      curiX=i;
      curiB = babo->seg[i]->iB ;
      curiE = babo->seg[i]->iE ;
      memcpy(curB,babo->seg[i]->cB,9);
      memcpy(curE,babo->seg[i]->cE,9);

      return 0;
    } //------- B 2 3

    //???C??????segE< tick==barE    ???  ==>  a             s    tick==barE
    //???D??????segE==tick==barE    ???  ==>  a             s    tick==barE
    //???4??????segE> barE==tick    ???  ==>  a             s    tick==barE
    if(memcmp(tik,barE,8)==0) {
      Display(fubo,tick,period,"eeee:2:C,D,4---");
aaa:
      // ?????? tick->UpdateMillisec < 500 ????????????tick??????K,?????????K+1 ?????????new K
      if(tick->UpdateMillisec < 500) {
        Display(fubo,tick,period,"eeee:002---");
        return 0;
      }

      if(SN==1 && memcmp(tik,SEGE,8)==0) {
        Display(fubo,tick,period,"eeee:003---");
        return 0;
      }

      // ??????tick == segE ??? tick->UpdateMillisec >= 500 , ???????????????seg??????????????????????????? tick ?????????
      // ????????????????????????????????? charmi
      if(curiX < babo->iSegNum -1) {
        if(memcmp(babo->seg[curiX]->cE,babo->seg[curiX+1]->cB,8)!=0) {
          Display(fubo,tick,period,"eeee:003---");
          return 0;
        }
      } else {  // ???????????? seg !!
        return 0;
      }

      // exexexex: ????????????????????????????????????????????????????????????seg????????????seg?????????
      // ?????? seg1:[09:00:00-10:00:00] seg2:[10:00:00-11:00:00] ???10:00:00????????????
      curiX++;
      goto ddd;

    } //------- C D 4

    //???E??????segE<=barE< tick    ???  ==>  b             s    tick> barE
    //???F??????tick <segE<=barE    ???  ==>  b + 0?        s    tick< barE             0?????????
    //???5??????segE >tick> barE    ???  ==>  b             s    tick> barE
    if((memcmp(SEGE,barE,8)<=0 && (memcmp(barE,tik,8)<0 || memcmp(tik,SEGE,8)<0)) ||
       (memcmp(SEGE,tik,8)>0 && memcmp(tik,barE,8)>0)) {
      Display(fubo,tick,period,"eeee:2EF5---");

bbb:
      if(memcmp(barE,"24:00:00",8)==0 && memcmp(tik,"00:00:00",8)==0 && tick->UpdateMillisec < 500) {
        Display(fubo,tick,period,"eeee:009---");
        return 0;
      }

      if(curiX < babo->iSegNum-1) {  //??????????????????????????????
        i = curiX;
      } else {
        i = 0;
      }

ccc:
      while(memcmp(tik,babo->seg[i]->cB,8)<0 || memcmp(tik,babo->seg[i]->cE,8)>0) {
        Display(fubo,tick,period,"eeee:010---");
        i++;
        if(i >= babo->iSegNum) {
          return 0;
        }
      }  // ?????? tick ???????????????
      curiX = i ;  // tick????????????
ddd:
      //if(curiX <= babo->iSegNum-1) {
      // ???tick?????????????????????????????????????????? bar????????????bar???????????? mark ==0 ???
      // ???????????? ?????? memcmp(tik,barE,8)==0 ?????????????????? ??????tick?????????bar?????????????????????
      // ???????????????????????? SendBar() SaveBar() ---> goto mark0 ???
      // ???????????? goto mark0;
      if(MARK==0) {
        //i=0;
        curiB = babo->seg[curiX]->iB-fr ;
        curiE = babo->seg[curiX]->iB ;
        do {
          curiB +=fr ;
          curiE +=fr ;
          uBEE::MakeTime(curB,curiB);
          uBEE::MakeTime(curE,curiE);
          //i++;
        } while(memcmp(tik,curB,8)<0 || memcmp(tik,curE,8)>0);

        NEW_B1;
        memcpy(barB,curB,9);
        memcpy(barE,curE,9);

        Display(fubo,tick,period,"eeee:011---");

        if(memcmp(tik,barE,8)==0) {
          Display(fubo,tick,period,"eeee:goo---0");
          SendBar(fubo,tick,period);
          SaveBar(fubo,tick,period);
          goto  mark0;
        }
        return 0;
      }

      // ??? mark >0 ???????????? tick???????????????????????????????????? (memcmp(tik,babo->seg[curiX]->cE,8)==0)
      // ??? ??????????????????????????????????????????????????? goto mark1 ;
      if(MARK > 0) {
        NEW_B1;
        curiB = babo->seg[curiX]->iB ;
        curiE = babo->seg[curiX]->iE ;
        memcpy(curB,babo->seg[curiX]->cB,9);
        memcpy(curE,babo->seg[curiX]->cE,9);
        memcpy(barB,babo->seg[curiX]->barB,9);
        memcpy(barE,babo->seg[curiX]->barE,9);

        Display(fubo,tick,period,"eeee:012---");

        if(memcmp(tik,babo->seg[curiX]->cE,8)==0) {
          Display(fubo,tick,period,"eeee:goo---1");
          SendBar(fubo,tick,period);
          SaveBar(fubo,tick,period);
          goto  mark1;
        }
        return 0;
      }
      return 0;
    } //-------- E F 5
  } //-------(MARK>0)


//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
  if(MARK < 0) {           // ?????????bar??????
    i = 0;
    goto ccc;
  }
  /*
    curB--curE
    barB--barE
    segB--segE
    mark ==> ??????seg?????????, 0:????????????seg????????????bar??? 1???2???3????????? ???seg????????????bar???
    idx  ==> seg ????????????
  */
  return 0;
}


TradingTime::TradingTime()       // constructor  new thread fot getting data APIs.
{
  TradingTime::Init(&t_hours[0]) ;
}

int TradingTime::Init(see_hours_t t_hours[])
{
  int             i ;
  int             k ;
  int             n ;
  int             x=0 ;
  FILE            *pf ;
  char            ca_segment[SEE_SGM_NUM][20] ;

  char            ca_30F_s[9] ;
  char            ca_1H_s[9] ;
  char            ca_2H_s[9] ;
  char            ca_3H_s[9] ;
  char            ca_4H_s[9] ;
  char            ca_5H_s[9] ;
  char            ca_1D_s[9] ;

  char            ca_30F_e[9] ;
  char            ca_1H_e[9] ;
  char            ca_2H_e[9] ;
  char            ca_3H_e[9] ;
  char            ca_4H_e[9] ;
  char            ca_5H_e[9] ;
  char            ca_1D_e[9] ;

  char            ca_line[1024] ;
  see_segment_t  *sgm ;

  //pf = fopen(TRADE_TIME,"r");
  pf = fopen("../etc/tbl/see_trade_time","r");
  if(pf == NULL) {
    uBEE::ErrLog(1000,"open trade time error!!",1,0,0) ;
    return -1 ;
  }

  for(i=0 ; i<SEE_HOUR_TYPE_NUM; i++) {
    for(k=0 ; k<SEE_SGM_NUM; k++) {
      t_hours[i].pt_segments[k] = NULL ;
    }
  }

  while(fgets(ca_line,1024, pf) != NULL) {
    memset(ca_30F_s,'\0',9);
    memset(ca_1H_s,'\0',9);
    memset(ca_2H_s,'\0',9);
    memset(ca_3H_s,'\0',9);
    memset(ca_4H_s,'\0',9);
    memset(ca_5H_s,'\0',9);
    memset(ca_1D_s,'\0',9);

    memset(ca_30F_e,'\0',9);
    memset(ca_1H_e,'\0',9);
    memset(ca_2H_e,'\0',9);
    memset(ca_3H_e,'\0',9);
    memset(ca_4H_e,'\0',9);
    memset(ca_5H_e,'\0',9);
    memset(ca_1D_e,'\0',9);

    k = string_split(ca_line,ca_segment) ;
    if(k<=2) {
      continue;
    }
    n = atoi(ca_segment[0]) ;   /* n ???????????????????????????????????????23?????????????????????23:30?????? */
    if(x<n+1) {
      x=n+1;
    }
    t_hours[n].i_hour_type= n ;
    for(i=1 ; i<k; i++) {
      sgm = (see_segment_t*)malloc(sizeof(see_segment_t)) ;
      t_hours[n].pt_segments[i-1] = sgm ;
      /* need to init sgm !!! */

      if(sgm == NULL) {
        uBEE::ErrLog(1000,"create k bar error!\n",1,0,0) ;
      }
      sgm->c_oc_flag = 'o' ;
      sgm->ca_b[8] = '\0' ;
      sgm->ca_e[8] = '\0' ;
      memcpy(sgm->ca_b,ca_segment[i],8) ;
      memcpy(sgm->ca_e,ca_segment[i]+9,8) ;

      if((i-1)%2  ==0) {
        memcpy(ca_30F_s,ca_segment[i],8);
      }
      if((i-1)%4  ==0) {
        memcpy(ca_1H_s, ca_segment[i],8);
      }
      if((i-1)%8  ==0) {
        memcpy(ca_2H_s, ca_segment[i],8);
      }
      if((i-1)%12 ==0) {
        memcpy(ca_3H_s, ca_segment[i],8);
      }
      if((i-1)%16 ==0) {
        memcpy(ca_4H_s, ca_segment[i],8);
      }
      if((i-1)%20 ==0) {
        memcpy(ca_5H_s, ca_segment[i],8);
      }
      if(i-1 ==0) {
        memcpy(ca_1D_s,ca_segment[i],8);
      }

      int w ;
      if((i-1)%2  ==1 || i+1==k) {
        if((i-1)%2  ==1) {
          memcpy(ca_30F_e,ca_segment[i]+9,8);
          for(w=i-2; w<=i-1; w++) {
            memcpy(t_hours[n].pt_segments[w]->ca_30F_end,ca_30F_e,9);
            /*
                                    sprintf(ca_errmsg,"lllllllllllllll  n:%d   i-1:%d  w:%d 30F end: %s ca_segment[i]:%s",n,i-1,
                                            w,ca_30F_e,ca_segment[i]) ;
                                    uBEE::ErrLog(1000,ca_errmsg,1,0,0) ;
            */
          }
        }
        if(i+1==k) {
          memcpy(ca_30F_e,ca_segment[i]+9,8);
          for(w=i-1-((i-1)%2); w<=i-1; w++) {
            memcpy(t_hours[n].pt_segments[w]->ca_30F_end,ca_30F_e,9);
            /*
                                    sprintf(ca_errmsg,"lllllllllllllll  n:%d   i-1:%d  w:%d 30F end: %s ca_segment[i]:%s",n,i-1,
                                            w,ca_30F_e,ca_segment[i]) ;
                                    uBEE::ErrLog(1000,ca_errmsg,1,0,0) ;
            */
          }
        }
      }


      if((i-1)%4  ==3 || i+1==k) {
        if((i-1)%4  ==3) {
          memcpy(ca_1H_e, ca_segment[i]+9,8);
          for(w=i-4; w<=i-1; w++) {
            memcpy(t_hours[n].pt_segments[w]->ca_1H_end,ca_1H_e,9);
            /*
                                    sprintf(ca_errmsg,"lllllllllllllll  n:%d   i-1:%d  w:%d 1H end: %s ca_segment[i]:%s",n,i-1,
                                            w,ca_1H_e,ca_segment[i]) ;
                                    uBEE::ErrLog(1000,ca_errmsg,1,0,0) ;
            */
          }
        }
        if(i+1==k) {
          memcpy(ca_1H_e, ca_segment[i]+9,8);
          for(w=i-1-((i-1)%4); w<=i-1; w++) {
            memcpy(t_hours[n].pt_segments[w]->ca_1H_end,ca_1H_e,9);
            /*
                                    sprintf(ca_errmsg,"lllllllllllllll  n:%d   i-1:%d  w:%d 1H end: %s ca_segment[i]:%s",n,i-1,
                                            w,ca_1H_e,ca_segment[i]) ;
                                    uBEE::ErrLog(1000,ca_errmsg,1,0,0) ;
            */
          }
        }
      }


      if((i-1)%8  ==7 || i+1==k) {
        if((i-1)%8  ==7) {
          memcpy(ca_2H_e, ca_segment[i]+9,8);
          for(w=i-8; w<=i-1; w++) {
            memcpy(t_hours[n].pt_segments[w]->ca_2H_end,ca_2H_e,9);
            /*
                                    sprintf(ca_errmsg,"lllllllllllllll  n:%d   i-1:%d  w:%d 2H end: %s ca_segment[i]:%s",n,i-1,
                                            w,ca_2H_e,ca_segment[i]) ;
                                    uBEE::ErrLog(1000,ca_errmsg,1,0,0) ;
            */
          }
        }
        if(i+1==k) {
          memcpy(ca_2H_e, ca_segment[i]+9,8);
          for(w=i-1-((i-1)%8); w<=i-1; w++) {
            memcpy(t_hours[n].pt_segments[w]->ca_2H_end,ca_2H_e,9);
            sprintf(ca_errmsg,"lllllllllllllll  n:%d   i-1:%d  w:%d 2H end: %s ca_segment[i]:%s",n,i-1,
                    w,ca_2H_e,ca_segment[i]) ;
            uBEE::ErrLog(1000,ca_errmsg,1,0,0) ;
          }
        }
      }

      if((i-1)%12 ==11 || i+1==k) {
        if((i-1)%12  ==11) {
          memcpy(ca_3H_e, ca_segment[i]+9,8);
          for(w=i-12; w<=i-1; w++) {
            memcpy(t_hours[n].pt_segments[w]->ca_3H_end,ca_3H_e,9);
            sprintf(ca_errmsg,"lllllllllllllll  n:%d   i-1:%d  w:%d 3H end: %s ca_segment[i]:%s",n,i-1,
                    w,ca_3H_e,ca_segment[i]) ;
            uBEE::ErrLog(1000,ca_errmsg,1,0,0) ;
          }
        }
        if(i+1==k) {
          memcpy(ca_3H_e, ca_segment[i]+9,8);
          for(w=i-1-((i-1)%12); w<=i-1; w++) {
            memcpy(t_hours[n].pt_segments[w]->ca_3H_end,ca_3H_e,9);
            sprintf(ca_errmsg,"lllllllllllllll  n:%d   i-1:%d  w:%d 3H end: %s ca_segment[i]:%s",n,i-1,
                    w,ca_3H_e,ca_segment[i]) ;
            uBEE::ErrLog(1000,ca_errmsg,1,0,0) ;
          }
        }
      }
      if((i-1)%16 ==15 || i+1==k) {
        if((i-1)%16  ==15) {
          memcpy(ca_4H_e, ca_segment[i]+9,8);
          for(w=i-16; w<=i-1; w++) {
            memcpy(t_hours[n].pt_segments[w]->ca_4H_end,ca_4H_e,9);
            sprintf(ca_errmsg,"lllllllllllllll  n:%d   i-1:%d  w:%d 4H end: %s ca_segment[i]:%s",n,i-1,
                    w,ca_4H_e,ca_segment[i]) ;
            uBEE::ErrLog(1000,ca_errmsg,1,0,0) ;
          }
        }
        if(i+1==k) {
          memcpy(ca_4H_e, ca_segment[i]+9,8);
          for(w=i-1-((i-1)%16); w<=i-1; w++) {
            memcpy(t_hours[n].pt_segments[w]->ca_4H_end,ca_4H_e,9);
            sprintf(ca_errmsg,"lllllllllllllll  n:%d   i-1:%d  w:%d 4H end: %s ca_segment[i]:%s",n,i-1,
                    w,ca_4H_e,ca_segment[i]) ;
            uBEE::ErrLog(1000,ca_errmsg,1,0,0) ;
          }
        }
      }
      if((i-1)%20 ==19 || i+1==k) {
        if((i-1)%20 ==19) {
          memcpy(ca_5H_e, ca_segment[i]+9,8);
          for(w=i-20; w<=i-1; w++) {
            memcpy(t_hours[n].pt_segments[w]->ca_5H_end,ca_5H_e,9);
            sprintf(ca_errmsg,"lllllllllllllll  n:%d   i-1:%d  w:%d 5H end: %s ca_segment[i]:%s",n,i-1,
                    w,ca_5H_e,ca_segment[i]) ;
            uBEE::ErrLog(1000,ca_errmsg,1,0,0) ;
          }
        }
        if(i+1==k) {
          memcpy(ca_5H_e, ca_segment[i]+9,8);
          for(w=i-1-((i-1)%20); w<=i-1; w++) {
            memcpy(t_hours[n].pt_segments[w]->ca_5H_end,ca_5H_e,9);
            sprintf(ca_errmsg,"lllllllllllllll  n:%d   i-1:%d  w:%d 5H end: %s ca_segment[i]:%s",n,i-1,
                    w,ca_5H_e,ca_segment[i]) ;
            uBEE::ErrLog(1000,ca_errmsg,1,0,0) ;
          }
        }
      }
      if(i+1 ==k) {
        memcpy(ca_1D_e,ca_segment[i]+9,8);
        for(w=0; w<=i-1; w++) {
          memcpy(t_hours[n].pt_segments[w]->ca_1D_end,ca_1D_e,9);
        }
      }

      memcpy(sgm->ca_15F_start,ca_segment[i],8) ;
      memcpy(sgm->ca_15F_end,  ca_segment[i]+9,8) ;

      /* need to be modify ! */
      memcpy(sgm->ca_15F_start,ca_segment[i],8) ;
      memcpy(sgm->ca_15F_end,  ca_segment[i]+9,8) ;

      memcpy(sgm->ca_30F_start,ca_30F_s,9) ;
      memcpy(sgm->ca_30F_end,  ca_30F_e,9) ;

      memcpy(sgm->ca_1H_start,ca_1H_s,9) ;
      memcpy(sgm->ca_1H_end,  ca_1H_e,9) ;

      memcpy(sgm->ca_2H_start,ca_2H_s,9) ;
      memcpy(sgm->ca_2H_end,  ca_2H_e,9) ;

      memcpy(sgm->ca_3H_start,ca_3H_s,9) ;
      memcpy(sgm->ca_3H_end,  ca_3H_e,9) ;

      memcpy(sgm->ca_4H_start,ca_4H_s,9) ;
      memcpy(sgm->ca_4H_end,  ca_4H_e,9) ;

      memcpy(sgm->ca_5H_start,ca_5H_s,9) ;
      memcpy(sgm->ca_5H_end,  ca_5H_e,9) ;

      memcpy(sgm->ca_1D_start,ca_1D_s,9) ;
      memcpy(sgm->ca_1D_end,  ca_1D_e,9) ;
      /*    */

      t_hours[n].pt_segments[i-1] = sgm ;
      sprintf(ca_errmsg,"-iiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiii n: %d, i-1: %d,%s,1D start: %s,  1D end : %s ",n, i-1,
              ca_segment[i],
              t_hours[n].pt_segments[i-1]->ca_1D_start,
              t_hours[n].pt_segments[i-1]->ca_1D_end) ;
      uBEE::ErrLog(1000,ca_errmsg,1,0,0) ;
    }

    for(i=1 ; i<k; i++) {
    }

    t_hours[n].pt_segments[k-1] = NULL ;
    t_hours[x].pt_segments[0] = NULL;
    sprintf(ca_errmsg,"ummmmmmmmmmmmmmm n:%d  x:%d  k:%d",n,x,k) ;
    uBEE::ErrLog(1000,ca_errmsg,1,0,0) ;
  }

  fclose(pf);
  return 0 ;
}

int string_split(char *s,char _cmd[SEE_SGM_NUM][20])
{
  char *p = s;
  int i = 0;
  int j = 0;
  /*????????????????????????*/
  while(*p == ' ') {
    p++;
  }
  while(*p != '\n' && *p != '\0') {
    if(*p == ' ') {
      _cmd[i][j]='\0';
      i++;
      j = 0;
      p++;
      /*???????????????????????????*/
      while(*p == ' ' || *p == '-') {
        p++;
      }
    } else {
      _cmd[i][j] = *p;
      p++;
      j++;
    }
  }
  _cmd[i][j]='\0';
  return i+1;
}

int string_split1(char *s, char ** _cmd,char c_seperater)
{
  char *p = s;
  int i = 0;
  int j = 0;

  while(*p == c_seperater) {
    p++;
  }

  while(*p != '\n' && *p != '\0') {
    if(*p == c_seperater) {
      _cmd[i][j]='\0';
      i++;
      j = 0;
      p++;
      /*???????????????????????????*/
      while(*p == c_seperater) {
        p++;
      }
    } else {
      _cmd[i][j] = *p;
      p++;
      j++;
    }
  }
  _cmd[i][j]='\0';
  return i+1;
}

FuBlock::FuBlock()       // constructor  new thread fot getting data APIs.
{
}


int FuBlock::Init(see_fut_block_t * p_block, char * pc_future, see_hours_t t_hours[])
{
  std::cout << "enter Init :" << pc_future << std::endl;
  int             i = 0 ;
  char            ca_future[FUTRUE_ID_LEN] ;
  char            ca_line[1024] ;
  FILE            *pf_trade_time ;
  FILE            *pf_future_time ;
  FILE            *pf_future_days ;
  see_bar_block_t     bar_block ;

  //memset(bar_block.ca_table, '\0', 128) ;
  //memset(bar_block.ca_home, '\0', 512) ;
  see_memzero(bar_block.ca_table,128) ;
  see_memzero(bar_block.ca_home, 512) ;

  bar_block.bar0.o = SEE_NULL;
  bar_block.bar1.o = SEE_NULL;
  bar_block.bar0.c = SEE_NULL;
  bar_block.bar1.c = SEE_NULL;
  bar_block.bar0.h = SEE_NULL;
  bar_block.bar1.h = SEE_NULL;
  bar_block.bar0.l = SEE_NULL;
  bar_block.bar1.l = SEE_NULL;
  bar_block.bar0.v = 0 ;
  bar_block.bar1.v = 0 ;
  bar_block.c_save = 0 ;


  memcpy(p_block->InstrumentID,pc_future,FUTRUE_ID_LEN) ;
  for(i=0; i<=30; i++) {
    memcpy((char *)&p_block->bar_block[i],&bar_block,sizeof(see_bar_block_t)) ;
  }

  for(i=0; i<8; i++) {
    p_block->bar_block[i].c_bar_type  = BAR_SECOND ;
  }
  for(i=8; i<16; i++) {
    p_block->bar_block[i].c_bar_type  = BAR_MINUTE ;
  }
  for(i=16; i<25; i++) {
    p_block->bar_block[i].c_bar_type  = BAR_HOUR ;
  }
  p_block->bar_block[25].c_bar_type = BAR_DAY ;
  p_block->bar_block[26].c_bar_type = BAR_WEEK ;
  p_block->bar_block[27].c_bar_type = BAR_MONTH ;
  p_block->bar_block[28].c_bar_type = BAR_SEASON ;
  p_block->bar_block[29].c_bar_type = BAR_YEAR ;
  p_block->bar_block[30].c_bar_type = BAR_TICKC ;

  p_block->bar_block[0].i_bar_type = 1 ;
  p_block->bar_block[1].i_bar_type = 2 ;
  p_block->bar_block[2].i_bar_type = 3 ;
  p_block->bar_block[3].i_bar_type = 5 ;
  p_block->bar_block[4].i_bar_type = 10 ;
  p_block->bar_block[5].i_bar_type = 15 ;
  p_block->bar_block[6].i_bar_type = 20 ;
  p_block->bar_block[7].i_bar_type = 30 ;

  p_block->bar_block[8].i_bar_type = 1 ;
  p_block->bar_block[9].i_bar_type = 2 ;
  p_block->bar_block[10].i_bar_type = 3 ;
  p_block->bar_block[11].i_bar_type = 5 ;
  p_block->bar_block[12].i_bar_type = 10 ;
  p_block->bar_block[13].i_bar_type = 15 ;
  p_block->bar_block[14].i_bar_type = 20 ;
  p_block->bar_block[15].i_bar_type = 30 ;

  for(i=16; i<22; i++) {
    p_block->bar_block[i].i_bar_type = i-15 ;
  }
  p_block->bar_block[22].i_bar_type = 8 ;
  p_block->bar_block[23].i_bar_type = 10 ;
  p_block->bar_block[24].i_bar_type = 12 ;

  for(i=25; i<=30; i++) {
    p_block->bar_block[i].i_bar_type = 1 ;
  }

  std::cout << " --pppp-- 22222\n";

  memset(ca_future,'\0',FUTRUE_ID_LEN);
  memcpy(ca_future,pc_future,2);
  if(!((ca_future[1] <= 'z' && ca_future[1] >= 'a') || (ca_future[1] <= 'Z' && ca_future[1] >= 'A'))) {
    ca_future[1] = '\0' ;
  }

  std::cout << " --pppp-- kkkkk\n";

  memset(p_block->ca_home, '\0', 512) ;
  sprintf(p_block->ca_home, "%s/see/dat/rcv_dat/%s", (char *)getenv("HOME"), ca_future) ;

  sprintf(p_block->bar_block[30].ca_table,"%s_tick", pc_future);
  sprintf(p_block->bar_block[30].ca_home,"%s/tick", p_block->ca_home);

  std::cout << " --pppp-- 33333\n";

  for(i=0; i<30; i++) {
    sprintf(p_block->bar_block[i].ca_table,"%s_%d%c", pc_future,
            p_block->bar_block[i].i_bar_type,
            p_block->bar_block[i].c_bar_type);
    sprintf(p_block->bar_block[i].ca_home,"%s/%d%c", p_block->ca_home,
            p_block->bar_block[i].i_bar_type,
            p_block->bar_block[i].c_bar_type);
  }

  std::cout << " --ppp -- \n";

  // -----------------------------------------------------
  // MAP M_FuTime : ??????????????? ??????????????? ?????????
  std::map<std::string,int>::const_iterator it;
  it = M_FuTime.find(ca_future);
  if(it==M_FuTime.end())
    std::cout<<"we do not find :"<< ca_future <<std::endl;
  else {
    p_block->i_hour_type = it->second ;
    p_block->pt_hour = &t_hours[p_block->i_hour_type] ;
  }
  //------------------------------------------------------


  std::cout << "--ooo-- after !" << std::endl;
  return SEE_OK ;
}




/*
 *  periods defined
 *  pp[31] ?????? ????????????period?????????????????? 3F ??? 180???
 */

/*                         0  1 2 3 4  5  6  7  8  9  10 11 12  13  14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30   */
/*                         1S 2 3 5 10 15 20 30 1F 2F 3F 5F 10F 15F 20 30 1H 2H 3H 4H 5H 6H 8H 10 12 1D 1W 1M 1J 1Y TICK */
static const int pp[31] = {1, 2,3,5,10,15,20,30,1, 2, 3, 5, 10, 15, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};


int
see_bars_calc_bar_1s(see_bar_t * bar)
{
  int rtn = 0;
  bar->o = 100;
  bar->c = 100;
  bar->h = 100;
  bar->l = 100;
  return rtn;
}

int
see_date_comp(char * pca_first, char * pca_last)
{

  int rc;
  char ca_first[9];
  char ca_last[9];
  if(strlen(pca_first) < 8) {
    uBEE::ErrLog(1000,"see_date_comp: input error!",RPT_TO_LOG,0,0);
  }
  if(strlen(pca_last) < 8) {
    uBEE::ErrLog(1000,"see_date_comp: input error!",RPT_TO_LOG,0,0);
  }
  memset(ca_first,'\0',9);
  memset(ca_last,'\0',9);
  memcpy(ca_first,pca_first,8);
  memcpy(ca_last,pca_last,8);
  rc = strcmp(ca_first,ca_last);
  return rc;
}

int
see_time_comp(char * f, char * l)    // ???????????????????????? ?????????06:33:26
{
  char f_h[3];
  char f_m[3];
  char f_s[3];

  char l_h[3];
  char l_m[3];
  char l_s[3];

  see_memzero(f_h,3);
  see_memzero(f_m,3);
  see_memzero(f_s,3);
  see_memzero(l_h,3);
  see_memzero(l_m,3);
  see_memzero(l_s,3);

  int fh,fm,fs,lh,lm,ls;

  time_t f_sec;
  time_t l_sec;

  struct tm f_time_fields;
  struct tm l_time_fields;

  memcpy(f_h,f,2);
  memcpy(f_m,f+3,2);
  memcpy(f_s,f+6,2);

  memcpy(l_h,l,2);
  memcpy(l_m,l+3,2);
  memcpy(l_s,l+6,2);

  fh = atoi(f_h);
  fm = atoi(f_m);
  fs = atoi(f_s);

  lh = atoi(l_h);
  lm = atoi(l_m);
  ls = atoi(l_s);

  if(fh == lh && fm == lm) {
    sprintf(ca_errmsg,"kkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkk, %s,%s,%d",f,l,ls-fs);
    uBEE::ErrLog(1000,ca_errmsg,RPT_TO_LOG,0,0);
    return ls-fs;
  }

  f_time_fields.tm_mday = 0;
  f_time_fields.tm_mon  = 0;
  f_time_fields.tm_year = 0;
  f_time_fields.tm_hour = fh;
  f_time_fields.tm_min  = fm;
  f_time_fields.tm_sec  = fs;

  l_time_fields.tm_mday = 0;
  l_time_fields.tm_mon  = 0;
  l_time_fields.tm_year = 0;
  l_time_fields.tm_hour = lh;
  l_time_fields.tm_min  = lm;
  l_time_fields.tm_sec  = ls;

  f_sec = mktime(&f_time_fields);
  l_sec = mktime(&l_time_fields);

  sprintf(ca_errmsg,"kkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkk, %s,%s,%d",f,l,(int)(l_sec-f_sec));
  uBEE::ErrLog(1000,ca_errmsg,RPT_TO_LOG,0,0);
  return l_sec-f_sec;
}

see_bar_t *
see_create_bar(char * p_future_id,char c_period)
{
  see_bar_t * kkk;
  kkk = (see_bar_t *)malloc(sizeof(see_bar_t));              // ?????????future ??????2???bars ????????? 0???????????????bar,1????????????bar
  if(kkk == NULL) {
    uBEE::ErrLog(1000,"see_create_bar error!\n",RPT_TO_LOG,0,0);
  }
  memset(kkk->TradingDay,'\0',9);
  memset(kkk->ca_btime,'\0',9);
  memset(kkk->ActionDay,'\0',9);
  kkk->o = -1;
  kkk->c = -1;
  kkk->h = -1;
  kkk->l = -1;
  kkk->v = -1;

  return kkk;
}

/*
* i_sgm_idx?????????????????????????????????tick??????????????????????????????
* ????????????tick????????????????????????is_mkt_open????????? block?????? i_sgm_idx c_oc_flag
*/
int is_mkt_open(see_fut_block_t *p_block, TICK *tick)
{
  int j = 0;
  int b;
  int e;

  while(j < SEE_SGM_NUM) {
    if(p_block->pt_hour->pt_segments[j] != NULL) {
      b = memcmp(tick->UpdateTime, p_block->pt_hour->pt_segments[j]->ca_b,8);
      e = memcmp(tick->UpdateTime, p_block->pt_hour->pt_segments[j]->ca_e,8);
      if(b >=0 && e<0 && p_block->pt_hour->pt_segments[j]->c_oc_flag == 'o') {
        p_block->i_sgm_idx = j;
        p_block->c_oc_flag = 'o';
        return SEE_MKT_OPEN;
      }
      j++;
    } else {
      p_block->i_sgm_idx = 0;
      p_block->c_oc_flag = 'c';
      return SEE_MKT_CLOSE;
    }
  }
  p_block->i_sgm_idx = 0;
  p_block->c_oc_flag = 'c';
  return SEE_MKT_CLOSE;
}

int is_weekend(char * pc_day)    //?????????????????????
{
  struct tm * p_time;
  struct tm cur_time;
  time_t   rawtime;
  char ca_y[5];
  char ca_m[3];
  char ca_d[3];

  see_memzero(ca_y,5);
  see_memzero(ca_m,3);
  see_memzero(ca_d,3);

  memcpy(ca_y,pc_day,4);
  memcpy(ca_m,pc_day+4,2);
  memcpy(ca_d,pc_day+6,2);

  cur_time.tm_mday = atoi(ca_d);
  cur_time.tm_mon  = atoi(ca_m)-1;
  cur_time.tm_year = atoi(ca_y)-1900;
  cur_time.tm_hour = 0;
  cur_time.tm_min  = 0;
  cur_time.tm_sec  = 0;
  rawtime = mktime(&cur_time);

  p_time =localtime(&rawtime);
  if(p_time->tm_wday == 6 || p_time->tm_wday == 0) {
    return 1;
  } else {
    return 0;
  }
}
int is_holiday(char * pc_day)
{
  return 0;
}

int is_notrade(see_fut_block_t * p_block,char * time0)    //?????????????????????
{
  /*
  */
  return 1;  //????????????????????????
}

/*
  see_handle_bars()
  p_block ???t_conf??????????????? ??????????????? p_block

  ???????????????(see_recivev.c, mds/src/ctpget.cpp) ?????????????????????????????????.

  ???????????? see_handle_bars ??? CalcBar() ?????? see_save_bar()???
  ?????? send to python ??? send to web???????????????????????????????????????????????????????????????????????????????????????
  ????????????????????? python rose????????????rose???????????????????????????????????????

  ????????????????????? see_handle_bars() ????????? ?????? ????????? ????????????????????? python rose????????????
  python rose ??????????????????????????????????????? ???????????????????????????????????????
  ????????????????????????????????????

*/
int
see_handle_bars(see_fut_block_t *p_block, TICK *tick)
{
  int     rc;
  int     period;

  rc = is_mkt_open(p_block,tick);      // ?????? block->c_oc_flag ?????? block->i_sgm_i_idx
  if(rc == SEE_MKT_CLOSE) {
    return 0;
  }

  /* -----------------------  ???????????? ------------------------- /
  if( memcmp(ca_UpdateTime,"22:59",5) ==0 && memcmp(tick->UpdateTime,"23:29",5) ==0  ) {return 0;} //????????????
  if( memcmp(tick->TradingDay,ca_TradingDay,8) ==0 )
  {
      uiui = see_time_to_int(tick->UpdateTime);
      mimi = see_time_to_int(ca_UpdateTime);
      if ( uiui < mimi ) { return 0; }
  }  // ????????????????????????K?????????????????????K???????????????????????????????????????
  memcpy(ca_TradingDay,tick->TradingDay,8);
  memcpy(ca_UpdateTime,tick->UpdateTime,8);
  / -----------------------  ???????????? ------------------------- */

  /*
  if(gp_conf->i_save_tick_only != 1) {
    for(period=0; period<=30; period++) {
      CalcBar(p_block, tick, period);                   // ??????K??? .
      see_save_bar(p_block, tick, period);                         // ????????????.
    }
  } else {
    see_save_bar(p_block, tick, BAR_TICK);
  }
  */
  for(period=0; period<=30; period++) {
    CalcBar(p_block, tick, period);                   // ??????K??? .
    // testing !!
    see_save_bar(p_block, tick, period);                         // ????????????.
  }
  return 0;
}

/*
rtn = 0; ????????? ???0?????????0???tick,???????????????tick???volume?????????????????????K??????
rtn = 1; ????????? ???????????????0?????????0???tick???????????????tick???volume?????????????????????K??????
*/
int
see_first_tick(see_fut_block_t *p_block,
               TICK            *tick,
               see_bar_t       *p_bar0,  //??????????????????
               see_bar_t       *p_bar1,
               int             period)
{
  int  rtn = 0;
  char f_h[3];
  char f_m[3];
  char f_s[3];
  see_memzero(f_h,3);
  see_memzero(f_m,3);
  see_memzero(f_s,3);

  int mo;
//    int fh;
  int fm;
  int fs;
  char * f;

  int  i_sgm_idx = p_block->i_sgm_idx;

  //memcpy(p_bar1->TradingDay,tick->TradingDay,8);
  //memcpy(p_bar1->ActionDay,tick->ActionDay,8);

  f = tick->UpdateTime;
  memcpy(f_h,f,2);
  memcpy(f_m,f+3,2);
  memcpy(f_s,f+6,2);

//    fh = atoi(f_h);
  fm = atoi(f_m);
  fs = atoi(f_s);

  switch(period) {
  case  BAR_TICK :
    break;

  case  BAR_1S :
  case  BAR_2S :
  case  BAR_3S :
  case  BAR_5S :
  case  BAR_10S :
  case  BAR_15S :
  case  BAR_20S :
  case  BAR_30S :
    NEW_BAR1;
    /*
    // ??????????????????
      ?????? ?????????tick?????????  21:16:21, fs = 21
      ????????????10S ???K??????????????????????????? fs%10 = 1, ?????? fs=fs-1 = 20, ?????????K????????????????????? 21:16:20
      ????????????20S ???K??????????????????????????? fs%20 = 1, ?????? fs=fs-1 = 20, ?????????K????????????????????? 21:16:20

      ????????????30S ???K??????????????????????????? fs%30 =21, ?????? fs=fs-21 =00, ?????????K????????????????????? 21:16:00
          ?????????????????????????????? 21:16:21???tick????????????30S???K????????????????????? ??? 21:16:00 ??? 21:16:21 ???
          ??????????????????????????????????????????21:16:21???tick??? ??????????????????
    */
    mo = fs%(pp[period]);    // ?????????????????????  21%30=21  21%10=1 21%20=1 21%5=1
    fs = fs - mo;
    if(mo == 0) {
      if(tick->UpdateMillisec == 0) {
        rtn = 0;
      } else {
        rtn = 1;
      }
    } else {
      memset(f_s,'\0',3);
      sprintf(f_s,"%02d",fs);
      memcpy(p_bar1->ca_btime+6,f_s,2);
      rtn = 1;
    }
    int bh,bm,bs;
    char temp[3];
    temp[2] = '\0';

    memcpy(temp,p_bar1->ca_btime,2);
    bh = atoi(temp);
    memcpy(temp,p_bar1->ca_btime+3,2);
    bm = atoi(temp);
    memcpy(temp,p_bar1->ca_btime+6,2);
    bs = atoi(temp);
    p_bar1->i_btime = bh*3600 + bh*60 + bs ;

    break;

  case  BAR_1F :
  case  BAR_2F :
  case  BAR_3F :
  case  BAR_5F :
  case  BAR_10F :
    NEW_BAR1;
    mo = fm%(pp[period]);
    fm = fm - mo;
    if(mo==0 && memcmp(tick->UpdateTime+6,"00",2)==0) {
      if(tick->UpdateMillisec == 0) {
        rtn = 0;
      } else {
        rtn = 1;
      }
    } else {
      memset(f_m,'\0',3);
      memset(f_s,'0',3);
      sprintf(f_m,"%02d",fm);
      memcpy(p_bar1->ca_btime+3,f_m,2);
      memcpy(p_bar1->ca_btime+6,f_s,2);
      rtn = 1;
    }
    break;

  case  BAR_15F :
    NEW_BAR1;
    memcpy(p_bar1->ca_btime,p_block->pt_hour->pt_segments[i_sgm_idx]->ca_15F_start,8);
    memcpy(p_bar1->ca_etime,p_block->pt_hour->pt_segments[i_sgm_idx]->ca_15F_end,8);
    if(memcmp(tick->UpdateTime,p_bar1->ca_btime,8)==0) {
      if(tick->UpdateMillisec == 0) {
        rtn = 0;
      } else {
        rtn = 1;
      }
    }
    break;

  case  BAR_20F :
    NEW_BAR1;
    memcpy(p_bar1->ca_btime,p_block->pt_hour->pt_segments[i_sgm_idx]->ca_20F_start,8);
    memcpy(p_bar1->ca_etime,p_block->pt_hour->pt_segments[i_sgm_idx]->ca_20F_end,8);
    if(memcmp(tick->UpdateTime,p_bar1->ca_btime,8)==0) {
      if(tick->UpdateMillisec == 0) {
        rtn = 0;
      } else {
        rtn = 1;
      }
    }
    break;

  case  BAR_30F :
    NEW_BAR1;
    memcpy(p_bar1->ca_btime,p_block->pt_hour->pt_segments[i_sgm_idx]->ca_30F_start,8);
    memcpy(p_bar1->ca_etime,p_block->pt_hour->pt_segments[i_sgm_idx]->ca_30F_end,8);
    if(memcmp(tick->UpdateTime,p_bar1->ca_btime,8)==0) {
      if(tick->UpdateMillisec == 0) {
        rtn = 0;
      } else {
        rtn = 1;
      }
    }
    break;

  case  BAR_1H :
    NEW_BAR1;
    memcpy(p_bar1->ca_btime,p_block->pt_hour->pt_segments[i_sgm_idx]->ca_1H_start,8);
    memcpy(p_bar1->ca_etime,p_block->pt_hour->pt_segments[i_sgm_idx]->ca_1H_end,8);
    if(memcmp(tick->UpdateTime,p_bar1->ca_btime,8)==0) {
      if(tick->UpdateMillisec == 0) {
        rtn = 0;
      } else {
        rtn = 1;
      }
    }
    break;
  case  BAR_2H :
    NEW_BAR1;
    memcpy(p_bar1->ca_btime,p_block->pt_hour->pt_segments[i_sgm_idx]->ca_2H_start,8);
    memcpy(p_bar1->ca_etime,p_block->pt_hour->pt_segments[i_sgm_idx]->ca_2H_end,8);
    if(memcmp(tick->UpdateTime,p_bar1->ca_btime,8)==0) {
      if(tick->UpdateMillisec == 0) {
        rtn = 0;
      } else {
        rtn = 1;
      }
    }
    break;

  case  BAR_3H :
    NEW_BAR1;
    memcpy(p_bar1->ca_btime,p_block->pt_hour->pt_segments[i_sgm_idx]->ca_3H_start,8);
    memcpy(p_bar1->ca_etime,p_block->pt_hour->pt_segments[i_sgm_idx]->ca_3H_end,8);
    if(memcmp(tick->UpdateTime,p_bar1->ca_btime,8)==0) {
      if(tick->UpdateMillisec == 0) {
        rtn = 0;
      } else {
        rtn = 1;
      }
    }
    break;

  case  BAR_4H :
    NEW_BAR1;
    memcpy(p_bar1->ca_btime,p_block->pt_hour->pt_segments[i_sgm_idx]->ca_4H_start,8);
    memcpy(p_bar1->ca_etime,p_block->pt_hour->pt_segments[i_sgm_idx]->ca_4H_end,8);
    if(memcmp(tick->UpdateTime,p_bar1->ca_btime,8)==0) {
      if(tick->UpdateMillisec == 0) {
        rtn = 0;
      } else {
        rtn = 1;
      }
    }
    break;

  case  BAR_5H :
    NEW_BAR1;
    memcpy(p_bar1->ca_btime,p_block->pt_hour->pt_segments[i_sgm_idx]->ca_5H_start,8);
    memcpy(p_bar1->ca_etime,p_block->pt_hour->pt_segments[i_sgm_idx]->ca_5H_end,8);
    if(memcmp(tick->UpdateTime,p_bar1->ca_btime,8)==0) {
      if(tick->UpdateMillisec == 0) {
        rtn = 0;
      } else {
        rtn = 1;
      }
    }
    break;
  case  BAR_1D :
    NEW_BAR1;
    memcpy(p_bar1->ca_btime,p_block->pt_hour->pt_segments[i_sgm_idx]->ca_1D_start,8);
    memcpy(p_bar1->ca_etime,p_block->pt_hour->pt_segments[i_sgm_idx]->ca_1D_end,8);
    if(memcmp(tick->UpdateTime,p_bar1->ca_btime,8)==0) {
      if(tick->UpdateMillisec == 0) {
        rtn = 0;
      } else {
        rtn = 1;
      }
    }
    break;
  case  BAR_1W :
    NEW_BAR1;
    break;
  case  BAR_1M :
    NEW_BAR1;
    break;
  case  BAR_1J :
    NEW_BAR1;
    break;
  case  BAR_1Y :
    NEW_BAR1;
    break;
  default :
    break;
  }
  p_bar1->vsum = tick->Volume;
  return rtn;
}

int
CalcBar(see_fut_block_t *p_block, TICK *tick, int period)
{
  int             i_kbar_num;
  see_bar_t       *p_bar0;
  see_bar_t       *p_bar1;

  if(period == BAR_TICK) {
    return 0;
  }

  p_bar0 =  &p_block->bar_block[period].bar0;
  p_bar1 =  &p_block->bar_block[period].bar1;

  if(p_block->c_oc_flag == 'o') {    // ?????????????????????
    if(p_bar1->o == SEE_NULL) {      // ???????????????????????????tick
      see_first_tick(p_block,tick,p_bar0,p_bar1,period);
      memcpy((char *)p_bar0,p_bar1,sizeof(see_bar_t));
      return 0;
    }

    /*
        ??? is_same_k_bar() ?????? ?????? c_save ???????????????'s'??????tick?????????????????????K????????????save???
        'n'?????? ???????????????tick??????????????????K??????
        i_kbar_num ???is_same_k_bar()???????????? ???==0 ?????? ????????????K??????
        i_kbar_num == 2,???????????????????????????K?????? save??????????????? i_kbar_num?????????1????????????1??????
        ??????????????????????????????????????????tick??????
    */
    i_kbar_num = is_same_k_bar(p_block,p_bar1,tick,period);
    if(i_kbar_num == 0) {   //?????????K
      UPDATE_BAR1;
    } else { // ?????????K
      /*
          ?????????
          ??????????????????????????????5F???K???
          ?????????N???K???????????? tick???????????? 09:29:59 000,??????tick???????????????N???K???????????????????????????
          ???N???K??????????????????
          ????????????tick????????????????????????09:31:02 500, ????????????????????????????????????tick?????????????????????
          ???????????????N???K????????????????????????p_bar1?????????copy??? p_bar0??????p_bar0??????N???K?????????????????????

      */
      /*
        ??????i_kbar_num>1????????????????????????K????????????????????????????????? ????????????????????????????????????
        :::::??????????????????:::::
      */
      memcpy((char *)p_bar0,p_bar1,sizeof(see_bar_t));
      if(see_first_tick(p_block,tick,p_bar0,p_bar1,period) == 0) {    // ???K??????tick->UpdateTime?????????????????? ???????????????
        p_bar0->v    = tick->Volume - p_bar0->vsum;
        p_bar0->vsum = tick->Volume;
      } else {
        p_bar1->v    = tick->Volume - p_bar0->vsum;
      }   // ?????? see_first_tick ?????? tick??? UpdateMillisec ??? 0 ?????? 500???????????? Volume

    }
  }
  return 0;
}

int is_same_k_bar(see_fut_block_t * p_block,
                  see_bar_t       * p_bar1,
                  TICK            *tick,
                  int             period)
{
  int  rc = 0;
  int  i_sgm_idx;

  //int  i_bar_type;
  //int  c_bar_type;

  char f_h[3];
  char f_m[3];
  char f_s[3];

  char b_h[3];
  char b_m[3];
  char b_s[3];

  see_memzero(f_h,3);
  see_memzero(f_m,3);
  see_memzero(f_s,3);
  see_memzero(b_h,3);
  see_memzero(b_m,3);
  see_memzero(b_s,3);

  int fh,fm,fs;
  int bh,bm,bs;

  char * f;
  char * b;

  if(p_bar1->o == SEE_NULL) {
    return -1;
  }

  i_sgm_idx  = p_block->i_sgm_idx;       // ???p_block?????? i_sgm_idx???
  //i_bar_type = p_block->bar_block[period].i_bar_type;
  //c_bar_type = p_block->bar_block[period].c_bar_type;


  b = p_bar1->ca_btime;     // ??????K?????????????????????
  memcpy(b_h,b,2);
  memcpy(b_m,b+3,2);
  memcpy(b_s,b+6,2);
  bh = atoi(b_h);
  bm = atoi(b_m);
  bs = atoi(b_s);
  f = tick->UpdateTime;     // ?????????tick?????????
  memcpy(f_h,f,2);
  memcpy(f_m,f+3,2);
  memcpy(f_s,f+6,2);
  fh = atoi(f_h);
  fm = atoi(f_m);
  fs = atoi(f_s);

  switch(period) {
  case  BAR_TICK :
    break;
  case  BAR_1S :
  case  BAR_2S :
  case  BAR_3S :
  case  BAR_5S :
  case  BAR_10S :
  case  BAR_20S :
  case  BAR_15S :
  case  BAR_30S :
    /*
      ?????? ca_btime ??? 13:59:48????????????????????? tick ??? 14:01:13
      ?????????????????????????????????????????????????????????

      ????????????????????????BAR_1S???K?????????????????????????????????K???????????????????????????????????????K?????????
      ?????????rc?????? = (14-13)*3600 + (01-59)*60 + 13 - 48 = 85??? K

      ??????????????? BAR_3S :
      rc = ((14-13)*3600 + (01-59)*60 + 13 - 48 )/3 = 28 ??? K???
      ???????????????28???K????????????????????????????????????????????? -----,?????????????????????????????????

    */
    //fs = fs - fs%(pp[period]);
    rc = ((fh-bh)*3600+(fm-bm)*60+fs-bs)/(pp[period]);
    break;
  case  BAR_1F :
  case  BAR_2F :
  case  BAR_3F :
  case  BAR_5F :
  case  BAR_10F :
    //fm = fm - fm%(pp[period]);
    rc = ((fh-bh)*60+fm-bm)/(pp[period]);
    break;

  case  BAR_15F :
    if(memcmp(p_bar1->ca_btime,p_block->pt_hour->pt_segments[i_sgm_idx]->ca_15F_start,8) ==0 ||
       memcmp(p_bar1->ca_etime,p_block->pt_hour->pt_segments[i_sgm_idx]->ca_15F_end,8) ==0) {
      rc = 0;
    } else {
      rc = 1;
    }
    break;
  case  BAR_20F :
    if(memcmp(p_bar1->ca_btime,p_block->pt_hour->pt_segments[i_sgm_idx]->ca_20F_start,8) ==0 ||
       memcmp(p_bar1->ca_etime,p_block->pt_hour->pt_segments[i_sgm_idx]->ca_20F_end,8) ==0) {
      rc = 0;
    } else {
      rc = 1;
    }
    break;
  case  BAR_30F :
    if(memcmp(p_bar1->ca_btime,p_block->pt_hour->pt_segments[i_sgm_idx]->ca_30F_start,8) ==0 ||
       memcmp(p_bar1->ca_etime,p_block->pt_hour->pt_segments[i_sgm_idx]->ca_30F_end,8) ==0) {
      rc = 0;
    } else {
      rc = 1;
    }
    break;
  case  BAR_1H :
    if(memcmp(p_bar1->ca_btime,p_block->pt_hour->pt_segments[i_sgm_idx]->ca_1H_start,8) ==0 ||
       memcmp(p_bar1->ca_etime,p_block->pt_hour->pt_segments[i_sgm_idx]->ca_1H_end,8) ==0) {
      rc = 0;
    } else {
      rc = 1;
    }
    break;
  case  BAR_2H :
    if(memcmp(p_bar1->ca_btime,p_block->pt_hour->pt_segments[i_sgm_idx]->ca_2H_start,8) ==0 ||
       memcmp(p_bar1->ca_etime,p_block->pt_hour->pt_segments[i_sgm_idx]->ca_2H_end,8) ==0) {
      rc = 0;
    } else {
      rc = 1;
    }
    break;
  case  BAR_3H :
    if(memcmp(p_bar1->ca_btime,p_block->pt_hour->pt_segments[i_sgm_idx]->ca_3H_start,8) ==0 ||
       memcmp(p_bar1->ca_etime,p_block->pt_hour->pt_segments[i_sgm_idx]->ca_3H_end,8) ==0) {
      rc = 0;
    } else {
      rc = 1;
    }
    break;
  case  BAR_4H :
    if(memcmp(p_bar1->ca_btime,p_block->pt_hour->pt_segments[i_sgm_idx]->ca_4H_start,8) ==0 ||
       memcmp(p_bar1->ca_etime,p_block->pt_hour->pt_segments[i_sgm_idx]->ca_4H_end,8) ==0) {
      rc = 0;
    } else {
      rc = 1;
    }
    break;
  case  BAR_5H :
    if(memcmp(p_bar1->ca_btime,p_block->pt_hour->pt_segments[i_sgm_idx]->ca_5H_start,8) ==0 ||
       memcmp(p_bar1->ca_etime,p_block->pt_hour->pt_segments[i_sgm_idx]->ca_5H_end,8) ==0) {
      rc = 0;
    } else {
      rc = 1;
    }
    break;
  case  BAR_1D :
    if(memcmp(p_bar1->ca_btime,p_block->pt_hour->pt_segments[i_sgm_idx]->ca_1D_start,8) ==0 ||
       memcmp(p_bar1->ca_etime,p_block->pt_hour->pt_segments[i_sgm_idx]->ca_1D_end,8) ==0) {
      rc = 0;
    } else {
      rc = 1;
    }
    break;
  case  BAR_1W :
    break;
  case  BAR_1M :
    break;
  case  BAR_1J :
    break;
  case  BAR_1Y :
    break;
  default :
    break;
  }
  if(rc ==0) {
    p_block->bar_block[period].c_save = 'n';
  } else {
    p_block->bar_block[period].c_save = 's';
  }
  return rc;
}

int SaveTick(see_fut_block_t *p_block,TICK *tick)
{
  char            ca_filename[512];

}


int see_save_bar(see_fut_block_t *p_block,
                 TICK *tick, int period)
{
  see_bar_t       *p_bar0;
  see_tick_t      new_tick;
  char            ca_year[5] = "\0\0\0";
  char            ca_month[7] = "\0\0\0";
  char            ca_filename[512];
  char            ca_tickname[512];
  char            ca_msg[1024];

  memset(ca_msg,'\0',1024);
  memset(ca_filename,'\0',512);

  if(period == BAR_TICK) {
    memset(ca_filename,'\0',512);
    sprintf(ca_filename,"%s/tick/%s-%s-tick",   p_block->ca_home,
            tick->InstrumentID,
            tick->TradingDay);
    memset(ca_tickname,'\0',512);
    sprintf(ca_tickname,"%s/tick/%s-%s-bin",    p_block->ca_home,
            tick->InstrumentID,
            tick->TradingDay);
    double dd;
    int    ii;
    char ca_Volume[100];
    char ca_PreClosePrice[100];
    char ca_OpenPrice[100];
    char ca_HighestPrice[100];
    char ca_LowestPrice[100];
    char ca_ClosePrice[100];
    char ca_AveragePrice[100];
    char ca_LastPrice[100];
    char ca_BidPrice1[100];
    char ca_BidVolume1[100];
    char ca_AskPrice1[100];
    char ca_AskVolume1[100];
    char ca_UpdateMillisec[100];

    char ca_errtmp[1024];


    /*
     * ????????????tick?????????
    */
    see_memzero(&new_tick,sizeof(see_tick_t));
    memcpy(&new_tick,tick,sizeof(TICK));
    struct timeval tv;
    gettimeofday(&tv,NULL);
    new_tick.rcv_msec = tv.tv_usec/1000 ;
    struct tm *t;
    time_t tt;
    time(&tt);
    t=localtime(&tt);
    sprintf(new_tick.rcv_date,"%4d%02d%02d",t->tm_year+1900,t->tm_mon+1,t->tm_mday);
    sprintf(new_tick.rcv_time,"%02d:%02d:%02d",t->tm_hour,t->tm_min,t->tm_sec);
    new_tick.rcv_week = t->tm_wday;


    ii = tick->Volume;
    if(ii < 10000000 && ii > -10000000) {
      memset(ca_Volume,'\0',100);
      sprintf(ca_Volume,"%d",ii);
    } else {
      sprintf(ca_Volume,"%s",(char *)"NULL");
    }

    dd = tick->PreClosePrice;
    if(dd < 10000000000 && dd > -10000000000) {
      memset(ca_PreClosePrice,'\0',100);
      sprintf(ca_PreClosePrice,"%10.2f",dd);
    } else {
      sprintf(ca_PreClosePrice,"%s",(char *)"NULL");
    }

    dd = tick->OpenPrice;
    if(dd < 10000000000 && dd > -10000000000) {
      memset(ca_OpenPrice,'\0',100);
      sprintf(ca_OpenPrice,"%10.2f",dd);
    } else {
      sprintf(ca_OpenPrice,"%s",(char *)"NULL");
    }

    dd = tick->HighestPrice;
    if(dd < 10000000000 && dd > -10000000000) {
      memset(ca_HighestPrice,'\0',100);
      sprintf(ca_HighestPrice,"%10.2f",dd);
    } else {
      sprintf(ca_HighestPrice,"%s",(char *)"NULL");
    }

    dd = tick->LowestPrice;
    if(dd < 10000000000 && dd > -10000000000) {
      memset(ca_LowestPrice,'\0',100);
      sprintf(ca_LowestPrice,"%10.2f",dd);
    } else {
      sprintf(ca_LowestPrice,"%s",(char *)"NULL");
    }

    dd = tick->ClosePrice;
    if(dd < 10000000000 && dd > -10000000000) {
      memset(ca_ClosePrice,'\0',100);
      sprintf(ca_ClosePrice,"%10.2f",dd);
    } else {
      sprintf(ca_ClosePrice,"%s",(char *)"NULL");
    }

    dd = tick->AveragePrice;
    if(dd < 10000000000 && dd > -10000000000) {
      memset(ca_AveragePrice,'\0',100);
      sprintf(ca_AveragePrice,"%10.2f",dd);
    } else {
      sprintf(ca_AveragePrice,"%s",(char *)"NULL");
    }

    dd = tick->LastPrice;
    if(dd < 10000000000 && dd > -10000000000) {
      memset(ca_LastPrice,'\0',100);
      sprintf(ca_LastPrice,"%10.2f",dd);
    } else {
      sprintf(ca_LastPrice,"%s",(char *)"NULL");
    }

    dd = tick->BidPrice1;
    if(dd < 10000000000 && dd > -10000000000) {
      memset(ca_BidPrice1,'\0',100);
      sprintf(ca_BidPrice1,"%10.2f",dd);
    } else {
      sprintf(ca_BidPrice1,"%s",(char *)"NULL");
    }

    ii = tick->BidVolume1;
    if(ii < 10000000 && ii > -10000000) {
      memset(ca_BidVolume1,'\0',100);
      sprintf(ca_BidVolume1,"%d",ii);
    } else {
      sprintf(ca_BidVolume1,"%s",(char *)"NULL");
    }

    dd = tick->AskPrice1;
    if(dd < 10000000000 && dd > -10000000000) {
      memset(ca_AskPrice1,'\0',100);
      sprintf(ca_AskPrice1,"%10.2f",dd);
    } else {
      sprintf(ca_AskPrice1,"%s",(char *)"NULL");
    }

    ii = tick->AskVolume1;
    if(ii < 10000000 && ii > -10000000) {
      memset(ca_AskVolume1,'\0',100);
      sprintf(ca_AskVolume1,"%d",ii);
    } else {
      sprintf(ca_AskVolume1,"%s",(char *)"NULL");
    }


    ii = tick->UpdateMillisec;
    if(ii < 10000000 && ii > -10000000) {
      memset(ca_UpdateMillisec,'\0',100);
      sprintf(ca_UpdateMillisec,"%d",ii);
    } else {
      sprintf(ca_UpdateMillisec,"%s",(char *)"NULL");
    }

    memset(ca_errtmp,'\0',1024);
    sprintf(ca_errtmp,"%s %s %s %s %s - %s %s %d %d -",tick->InstrumentID,
            tick->TradingDay,
            tick->ActionDay,
            tick->UpdateTime,
            ca_UpdateMillisec,
            new_tick.rcv_date,
            new_tick.rcv_time,
            new_tick.rcv_msec,
            new_tick.rcv_week);

    sprintf(ca_msg,"%s H:%s L:%s LastP:%s B1:%s BV1:%s A1:%s AV1:%s V:%s\n",
            ca_errtmp,
            ca_HighestPrice,ca_LowestPrice,ca_LastPrice,
            ca_BidPrice1,ca_BidVolume1,ca_AskPrice1,ca_AskVolume1,
            ca_Volume);
    // testing !!
    std::cout << ca_msg << std::endl;
    //SaveLine(ca_filename,ca_msg);

    //SaveBin(ca_tickname,(char *)&new_tick,sizeof(see_tick_t));
    return 0;
  } /* tick data saving */


  p_bar0 =  &p_block->bar_block[period].bar0;
  memcpy(ca_year,p_bar0->TradingDay,4);
  memcpy(ca_month,p_bar0->TradingDay,6);

  if(p_block->bar_block[period].c_save == 's') {
    memset(ca_filename,'\0',512);
    if(period <=7) {
      sprintf(ca_filename,"%s/%s-%s-%d%c",    p_block->bar_block[period].ca_home,
              p_block->InstrumentID,
              p_block->bar_block[period].bar0.TradingDay,
              p_block->bar_block[period].i_bar_type,
              p_block->bar_block[period].c_bar_type);
    } else if(period <=15) {
      sprintf(ca_filename,"%s/%s-%s-%d%c",    p_block->bar_block[period].ca_home,
              p_block->InstrumentID,
              ca_month,
              p_block->bar_block[period].i_bar_type,
              p_block->bar_block[period].c_bar_type);
    } else {
      sprintf(ca_filename,"%s/%s-%s-%d%c",    p_block->bar_block[period].ca_home,
              p_block->InstrumentID,
              ca_year,
              p_block->bar_block[period].i_bar_type,
              p_block->bar_block[period].c_bar_type);
    }
    sprintf(ca_msg,"%s %s %s %s %10.2f %10.2f %10.2f %10.2f %d\n",
            p_block->InstrumentID,
            p_block->bar_block[period].bar0.TradingDay,
            p_block->bar_block[period].bar0.ActionDay,
            p_bar0->ca_btime,p_bar0->o,p_bar0->h,p_bar0->l,p_bar0->c,p_bar0->v);

    // testing !!
    std::cout << ca_msg << std::endl;
    //SaveLine(ca_filename,ca_msg);

    char ca_sss[1024];
    memset(ca_sss,'\0',1024);
    /*
    sprintf(ca_sss,"{\"topic\":\"%s\",\"data\":\"%s\"}",p_block->InstrumentID,
            ca_msg);
    */
    sprintf(ca_sss,"{\"topic\":\"%s:%d%c\",\"data\":\"%s\"}",p_block->InstrumentID,
            p_block->bar_block[period].i_bar_type,
            p_block->bar_block[period].c_bar_type,ca_msg);
    //see_send_bar(p_block,ca_sss);
  }
  return 0;
}
/*
 see_send_bar() ???web python ??????bar????????????
*/

/*
    ?????????see_fut_block_t *p_block
    ??????????????? future block???

    ?????????
    ??? p_block???????????????period??? K?????????????????????????????????
*/


/*
 *  if c_save == 's' , send bar1->o h l c v,  to hh[] oo[] ll[] cc[] vv[]  marked : this is a new K !!
 *  if c_save == 'n' , send bar1->o h l c v,  to hh[] oo[] ll[] cc[] vv[]  marked : this is a updating K !!
 *  send to another process for KDJ? for strategy ?  or calculate KDJ locally ?
*/


int see_save_bar_last(see_fut_block_t *p_block, int period, int i_another_day)
{
  see_bar_t       *p_bar0;
  see_bar_t       *p_bar1;
  char            ca_year[5];
  char            ca_month[7];
  char            ca_filename[512];
  char            ca_msg[1024];

  if(period == BAR_TICK) {
    return 0;
  }

  p_bar0 =  &p_block->bar_block[period].bar0;
  p_bar1 =  &p_block->bar_block[period].bar1;

  memcpy((char *)p_bar0,p_bar1,sizeof(see_bar_t));
  /*
  if( see_first_tick(p_block,buf,p_bar0,p_bar1,period) == 0 ) {  // ???K??????tick->UpdateTime?????????????????? ???????????????
      p_bar0->v    = tick->Volume - p_bar0->vsum;
      p_bar0->vsum = tick->Volume;
  }else{
      p_bar1->v    = tick->Volume - p_bar0->vsum;
  }
  */

  memset(ca_msg,'\0',1024);
  memset(ca_filename,'\0',512);


  p_bar0 =  &p_block->bar_block[period].bar0;
  memcpy(ca_year,p_bar0->TradingDay,4);
  memcpy(ca_month,p_bar0->TradingDay,6);

  memset(ca_filename,'\0',512);
  if(period <=7) {
    sprintf(ca_filename,"%s/%d%c/%s-%s-%d%c",    p_block->ca_home,
            p_block->bar_block[period].i_bar_type,
            p_block->bar_block[period].c_bar_type,
            p_block->InstrumentID,
            p_block->bar_block[period].bar0.TradingDay,
            p_block->bar_block[period].i_bar_type,
            p_block->bar_block[period].c_bar_type);
  } else if(period <=15) {
    sprintf(ca_filename,"%s/%d%c/%s-%s-%d%c",    p_block->ca_home,
            p_block->bar_block[period].i_bar_type,
            p_block->bar_block[period].c_bar_type,
            p_block->InstrumentID,
            ca_month,
            p_block->bar_block[period].i_bar_type,
            p_block->bar_block[period].c_bar_type);
  } else {
    sprintf(ca_filename,"%s/%d%c/%s-%s-%d%c",    p_block->ca_home,
            p_block->bar_block[period].i_bar_type,
            p_block->bar_block[period].c_bar_type,
            p_block->InstrumentID,
            ca_year,
            p_block->bar_block[period].i_bar_type,
            p_block->bar_block[period].c_bar_type);
  }
  sprintf(ca_msg,"%s %s %s %s %10.2f %10.2f %10.2f %10.2f %d\n",
          p_block->InstrumentID,
          p_block->bar_block[period].bar0.TradingDay,
          p_block->bar_block[period].bar0.ActionDay,
          p_bar0->ca_btime,p_bar0->o,p_bar0->h,p_bar0->l,p_bar0->c,p_bar0->v);
  //uBEE::ErrLog(1000,ca_filename,RPT_TO_LOG,0,0 );
  SaveLine(ca_filename,ca_msg);
  if(i_another_day==1) {
    p_bar0->o = SEE_NULL;
  }
  return 0;
}


int MakeTime(char *caT, int T)
{
  int h,m,s;
  char cTmp[9];
  h = T / 3600;
  m =(T - h*3600) / 60;
  s = T % 60;
  see_memzero(cTmp,9);
  sprintf(cTmp,"%02d:%02d:%02d",h,m,s);
  memcpy(caT, cTmp, 9);
  return 0;
}

int SendBars()
{
  return 0;
}


int HandleTick(uBEE::FuBo *fubo, TICK *tick)
{
  int i = 0;


  for(i=0; i<50; ++i) {
    if(fubo->pBaBo[i] != nullptr) {
      SendBar(fubo,tick,i) ;
    }
  }

  for(i=0; i<50; ++i) {
    if(fubo->pBaBo[i] != nullptr) {
      SaveBar(fubo,tick,i) ;
      DealBar(fubo,tick,i) ;
    }
  }

  char f[512] ;
  snprintf(f,512,"../data/tick/%s.%s.tick.bin",fubo->InstrumentID,tick->TradingDay);
  SaveBin(f,(const char *)tick,sizeof(TICK)) ;

  snprintf(f,512,"../data/tick/%s.%s.tick.txt",fubo->InstrumentID,tick->TradingDay);
  sprintf(ca_errmsg,"T:%s %s %06d S:%d A:%s H:%g L:%g LP:%g AP:%g AV:%d BP:%g BV:%d OI:%g V:%d",
          tick->TradingDay,
          tick->UpdateTime,
          tick->UpdateMillisec*1000,
          0,
          tick->ActionDay,
          tick->HighestPrice,
          tick->LowestPrice,
          tick->LastPrice,
          tick->AskPrice1,
          tick->AskVolume1,
          tick->BidPrice1,
          tick->BidVolume1,
          tick->OpenInterest,
          tick->Volume);
  SaveLine(f,ca_errmsg) ;
  return 0;
}


int SaveBar(uBEE::FuBo *fubo, TICK *tick,int period)
{
  BaBo * babo = fubo->pBaBo[period] ;
  stBar *b1 = babo->b1 ;
  stBar *p_bar1 =  &babo->bar1;

  char f[512] ;

  if(b1->sent==1) {
    b1->sent =2 ;
    snprintf(f,512,"../data/%s_%02d_%02d_%02d.%d.%di",fubo->InstrumentID,babo->iH,babo->iM,babo->iS,babo->iF,period);
    snprintf(ca_errmsg,ERR_MSG_LEN,"%s T:%s A:%s %s--%s O:%g H:%g L:%g C:%g V:%d vsam:%d",
             fubo->InstrumentID, b1->TradingDay, b1->ActionDay,
             b1->cB, b1->cE,
             b1->o, b1->h, b1->l, b1->c,
             b1->v, b1->vsum) ;
    //uBEE::ErrLog(1000,ca_errmsg,1,0,0) ;
    SaveLine(f,ca_errmsg) ;
  }
  return 0;
}

int SendBar(uBEE::FuBo *fubo, TICK *tick,int period)
{
  //stBar       *p_bar0;
  stBar       *p_bar1;
  //stBar       *b0;
  stBar       *b1;
  stBar       *bt;

  //b0 = fubo->pBaBo[period]->b0 ;
  b1 = fubo->pBaBo[period]->b1 ;

  //p_bar0 =  &fubo->pBaBo[period]->bar0;
  p_bar1 =  &fubo->pBaBo[period]->bar1;

  char * curB = fubo->pBaBo[period]->curB ;
  char * curE = fubo->pBaBo[period]->curE ;
  int &curiB = fubo->pBaBo[period]->curiB ;
  int &curiE = fubo->pBaBo[period]->curiE ;
  int &curiX = fubo->pBaBo[period]->curiX ;

  char * barB = b1->cB ;
  char * barE = b1->cE ;

  BaBo * babo = fubo->pBaBo[period] ;


#define SEGB fubo->pBaBo[period]->seg[curiX]->cB
#define SEGE fubo->pBaBo[period]->seg[curiX]->cE
#define MARK babo->seg[curiX]->mark

  char * tik = tick->UpdateTime ;

  int mark ;

  int fr = fubo->pBaBo[period]->iF ;


  // ----------???????????? ---------------------------
  if(memcmp(barE,tik,8)==0) {
    if(SN==1 && memcmp(tik,SEGE,8)==0) {
      if(tick->UpdateMillisec < 500) {
        return 0;
      } else {
        UPDATE_B1;
        SendBars();
        b1->sent = 1;
        DispBar(fubo,tick,period,"ssss:0:A,C1---");
      }
      return 0;
    } else {
      if(tick->UpdateMillisec < 500) {
        UPDATE_B1;
        SendBars();
        b1->sent = 1;
        DispBar(fubo,tick,period,"ssss:0:A,C1---");
      } else {
        if(!b1->sent) {
          SendBars();
          b1->sent = 1;
          DispBar(fubo,tick,period,"ssss:0:A,C2---");
        }
      }
      return 0;
    }
  }

  if(memcmp(tick->UpdateTime,curB,8)>=0 &&
     memcmp(tick->UpdateTime,curE,8)<0) { // ???????????? curB curE ??????segment???
    //UPDATE_B1;
    return 0;
  }


  if(MARK == 0) {
    /*
    if(memcmp(barE,tik,8)==0) {
      // ???A??????barE==tick <segE  ???   ==>  e         s         {????????????bar}
      // ???C??????barE==tick==segE  ???   ==>  a         s         {????????????bar}
      if(tick->UpdateMillisec < 500) {
        UPDATE_B1;
        SendBars();
        b1->sent = 1;
        DispBar(fubo,tick,period,"ssss:0:A,C1---");
      } else {
        if(!b1->sent) {
          SendBars();
          b1->sent = 1;
          DispBar(fubo,tick,period,"ssss:0:A,C2---");
        }
      } //------- A C
      return 0;
    }
    */

    //???B??????barE <tick <segE  ???   ==>  f         s or 2    {??????????????????bar}
    //???D??????barE <tick==segE  ???   ==>  a         s +  2    {??????????????????bar}
    if(memcmp(barE,tik,8)< 0) {
      if(!b1->sent) {
        SendBars();
        b1->sent = 1;
        DispBar(fubo,tick,period,"ssss:0:B---");
      }
      return 0;
    } // ------- C D

    //???E??????barE<=segE <tick  ???   ==>  b         s
    if(memcmp(SEGE,tik,8)<0) {
      if(!b1->sent) {
        SendBars();
        b1->sent = 1;
        DispBar(fubo,tick,period,"ssss:0:E---");
      }
      return 0;
    } // ------- E

    //???F??????tick <barE<=segE  ???   ==>  b + 0?    s + 0?     {????????????bar}   1???0?????????
    if(memcmp(tik,barE,8)<0) {
      if(memcmp(barE,"24:00:00",8)==0 && memcmp(tik,"00:00:00",8)==0 && tick->UpdateMillisec < 500) {
        UPDATE_B1;
        SendBars();
        b1->sent = 1;
        DispBar(fubo,tick,period,"ssss:0:F1---");
      } else {
        if(!b1->sent) {
          SendBars();
          b1->sent = 1;
          DispBar(fubo,tick,period,"ssss:0:F2---");
        }
      }
      return 0;
    } //------- F

  } //------- MARK == 0

  if(MARK > 0) {
    /*
    //???C??????segE< tick==barE    ???  ==>  a             s    tick==barE
    //???D??????segE==tick==barE    ???  ==>  a             s    tick==barE
    //???4??????segE> barE==tick    ???  ==>  a             s    tick==barE
    if(memcmp(tik,barE,8)==0) {
      if(tick->UpdateMillisec < 500) {
        UPDATE_B1;
        SendBars();
        b1->sent = 1;
        DispBar(fubo,tick,period,"ssss:1:CD41---");
      } else {
        if(!b1->sent) {
          SendBars();
          b1->sent = 1;
          DispBar(fubo,tick,period,"ssss:1:CD42---");
        }
      }
      return 0;
    } // ------- C D 4
    */

    //???5??????segE >tick> barE    ???  ==>  b             s    tick> barE
    //???E??????segE<=barE< tick    ???  ==>  b             s    tick> barE
    if((memcmp(tik,barE,8)>0) && (memcmp(SEGE,tik,8)>0 || memcmp(SEGE,barE,8)<=0)) {
      if(!b1->sent) {
        SendBars();
        b1->sent = 1;
        DispBar(fubo,tick,period,"ssss:1:5E---");
      }
    } // ------- 5 E

    //???F??????tick <segE<=barE    ???  ==>  b + 0?        s    tick< barB             0?????????
    if(memcmp(tik,SEGE,8)<0 && memcmp(SEGE,barE,8)<=0) {
      if(memcmp(barE,"24:00:00",8)==0 && memcmp(tik,"00:00:00",8)==0 && tick->UpdateMillisec < 500) {
        UPDATE_B1;
        SendBars();
        b1->sent = 1;
        DispBar(fubo,tick,period,"ssss:1:5E---");
      } else {
        if(!b1->sent) {
          SendBars();
          b1->sent = 1;
          DispBar(fubo,tick,period,"ssss:1:F2---");
        }
      }
    } // ------- F
  } // ------- MARK > 0
}

int Display(uBEE::FuBo *fubo, TICK *tick,int period,const char*msg)
{
  BaBo * babo = fubo->pBaBo[period] ;

  //stBar *b0 = babo->b0 ;
  stBar *b1 = babo->b1 ;
  //stBar *p_bar0 =  &babo->bar0;
  stBar *p_bar1 =  &babo->bar1;

  char * curB = babo->curB ;
  char * curE = babo->curE ;
  int &curiB = babo->curiB ;
  int &curiE = babo->curiE ;
  int &curiX = babo->curiX ;
  int fr = babo->iF ;

  char * barB = b1->cB ;
  char * barE = b1->cE ;
  char * tik = tick->UpdateTime ;

  char * segB = babo->seg[curiX]->cB;
  char * segE = babo->seg[curiX]->cE;
  int  &mark  = babo->seg[curiX]->mark;


  if(T________) {
    sprintf(ca_errmsg,"%s p:%d fr:%d tick:%s ms:%d crBE:%s-%s brBE:%s-%s sgBE:%s-%s curiX:%d",
            msg,
            period,fr,tick->UpdateTime,tick->UpdateMillisec, curB,curE, barB,barE, SEGB,SEGE, curiX) ;
    uBEE::ErrLog(1000,ca_errmsg,1,0,0) ;
  }
  return 0;
}

int DispBar(uBEE::FuBo *fubo, TICK *tick,int period,const char*msg)
{
  BaBo * babo = fubo->pBaBo[period] ;
  //stBar *b0 = babo->b0 ;
  stBar *b1 = babo->b1 ;
  //stBar *p_bar0 =  &babo->bar0;
  stBar *p_bar1 =  &babo->bar1;

  if(T________) {
    sprintf(ca_errmsg,"%s T:%s A:%s B:%s--%s H:%lf O:%lf C:%lf L:%lf V:%d vsam:%d",
            msg, b1->TradingDay, b1->ActionDay,
            b1->cB, b1->cE,
            b1->h, b1->o, b1->c, b1->l,
            b1->v, b1->vsum) ;
    uBEE::ErrLog(1000,ca_errmsg,1,0,0) ;
  }
}

} //namespace

#include "uBEE.h"
#include <cjson/cJSON.h>
#include <iostream>
#include <thread>
#include <unistd.h>
#include <algorithm>
#include <vector>
#include<time.h>

namespace uBEE
{

FuList::FuList()
{
}

int FuList::isFirstDay(int type,int y,int m, int d)
{
  int rtc = -1;
  if(type <0 || type > 3) {
    std::cout << " type error !   [0]: CFFE  --- [1]: SHFE  --- [2]: CZCE  --- [3]: DCE  --- \n";
    return -1;
  }
  if(m<=0 || m>12) {
    std::cout << " month error !: " << m << std::endl;
    return -1;
  }
  if(d<=0 || d>31) {
    std::cout << " day error !: " << m << std::endl;
    return -1;
  }
  switch(y) {
  case 2016:
    rtc = (d<FirstDay16[m-1][type]) ? NON_FIRST_DAY:FIRST_DAY;
    return rtc;
    break;
  case 2017:
    rtc = (d<FirstDay17[m-1][type]) ? NON_FIRST_DAY:FIRST_DAY;
    return rtc;
    break;
  case 2018:
    rtc = (d<FirstDay18[m-1][type]) ? NON_FIRST_DAY:FIRST_DAY;
    return rtc;
    break;
  /*
  // 需要修改！！
  case 2019:
    rtc = (d<FirstDay19[m-1][type]) ? NON_FIRST_DAY:FIRST_DAY;
    return rtc;
    break;
  case 2020:
    rtc = (d<FirstDay20[m-1][type]) ? NON_FIRST_DAY:FIRST_DAY;
    return rtc;
    break;
  case 2021:
    rtc = (d<FirstDay21[m-1][type]) ? NON_FIRST_DAY:FIRST_DAY;
    return rtc;
    break;
  case 2022:
    rtc = (d<FirstDay22[m-1][type]) ? NON_FIRST_DAY:FIRST_DAY;
    return rtc;
    break;
  */
  default:
    std::cout << " year error !  year should be 2016 ,2017 or 2018 ! \n";
    return -1;
  }
}

int FuList::DateDeal(int y,int m,int d)
{
  int rtn = 0;
  int week = 0;

  /*
  rtn = CheckDate(y,m,d);
  if(rtn !=0) {
    std::cout << rtn << " DateDeal() Date is invalid!\n" ;
    return rtn;
  }
  */

  memset(Date,'\0',9);
  sprintf(Date,"%d%02d%02d",y,m,d); //member
  Year = y;          // member
  Month = m;         // member
  Day = d;           //member


  if(m==1) {
    m=13;
    y--;
  }
  if(m==2) {
    m=14;
    y--;
  }
  if((y<1752)||((y==1752)&&(m<9))||((y==1752)&&(m==9)&&(d<3))) 	//判断是否在1752年9月3日之前
    week =(d+2*m+3*(m+1)/5+y+y/4+5)%7; 							//1752年9月3日之前的公式
  else
    week =(d+2*m+3*(m+1)/5+y+y/4-y/100+y/400)%7; 				//1752年9月3日之后的公式

  Week = week+1;    //member
  return 0;

}

int FuList::Init(int y,int m, int d)
{
  int rtn;

  rtn = FuList::DateDeal(y,m,d);    //会初始化成员变量
  if(rtn != 0) {
    std::cout << "日期输入错误！\n" ;
    return -1;
  }
  if(6==Week || 7==Week) {
    std::cout << " not trading day, week: " << Week << std::endl;
    // 不是交易日期，也应该能生成 期货列表。
    //return -1;
  }

  for(int i = 0; i< DCE_NUMBER; i++) {
    memset(ShfeList[i],'\0',FUTRUE_ID_LEN);
    memset(DceList[i], '\0',FUTRUE_ID_LEN);
    memset(CzceList[i],'\0',FUTRUE_ID_LEN);
    pShfeList[i] = nullptr;
    pDceList[i]  = nullptr;
    pCzceList[i] = nullptr;
  }
  for(int i = 0; i< CFFE_NUMBER; i++) {
    memset(CffeList[i],'\0',FUTRUE_ID_LEN);
    pCffeList[i] = nullptr;
  }

  std::cout << "after FuList::DateDeal(pDate)" << Year << Month << Day << "-------\n";

  // [1][0]: CFFE  --- [1][1]: SHFE  --- [1][2]: CZCE  --- [1][3]: DCE  ---
  FuList::CffeListInit();
  FuList::ListInit(TYPE_SHFE);
  FuList::ListInit(TYPE_CZCE);
  FuList::ListInit(TYPE_DCE);
  FuList::pListInit();      // 指针数组
  std::cout << " FuList created !!\n" ;
  return 0;
}

void  FuList::CffeListInit()
{
  int isFd = isFirstDay(TYPE_CFFE,Year,Month,Day);
  int m1;
  int m2;
  int j1;
  int j2;
  int FuNum = 0;
  int y1= Year -2000;
  int y2= y1+1;
  char future[31] ;
  memset(future,'\0',31);
  std::cout << "isFirstDay: CFFE :" << isFd << std::endl;

  for(auto it = M_CFFE.begin(); it != M_CFFE.end(); ++it) {
    if(1==isFirstDay(TYPE_CFFE,Year,Month,Day)) {
      m1 = Month + 1;
      m2 = Month + 2;
    } else {
      m1 = Month ;
      m2 = Month + 1;
    }
    //        3     6     9       12       15     18     21     24
    //   1  2  4 5   7 8   10 11    13 14

    if("IF" == it->first || "IC" == it->first || "IH" == it->first) {
      if(m2 < 3) {
        for(int i = 0; i<2; i++) {
          memset(future,'\0',31);
          sprintf(future,"%s%d%02d",it->first.c_str(),y1,(m1+i));
          memcpy(CffeList[FuNum],future,strlen(future));
          FuNum ++;
        }
        for(int i = 0; i<2; i++) {
          memset(future,'\0',31);
          sprintf(future,"%s%d%02d",it->first.c_str(),y1,(3+i*3));
          memcpy(CffeList[FuNum],future,strlen(future));
          FuNum ++;
        }
      } else if(m2 < 6) {
        for(int i = 0; i<2; i++) {
          memset(future,'\0',31);
          sprintf(future,"%s%d%02d",it->first.c_str(),y1,(m1+i));
          memcpy(CffeList[FuNum],future,strlen(future));
          FuNum ++;
        }
        for(int i = 0; i<2; i++) {
          memset(future,'\0',31);
          sprintf(future,"%s%d%02d",it->first.c_str(),y1,(6+i*3));
          memcpy(CffeList[FuNum],future,strlen(future));
          FuNum ++;
        }
        // 6 9
      } else if(m2 < 9) {
        for(int i = 0; i<2; i++) {
          memset(future,'\0',31);
          sprintf(future,"%s%d%02d",it->first.c_str(),y1,(m1+i));
          memcpy(CffeList[FuNum],future,strlen(future));
          FuNum ++;
        }
        for(int i = 0; i<2; i++) {
          memset(future,'\0',31);
          sprintf(future,"%s%d%02d",it->first.c_str(),y1,(9+i*3));
          memcpy(CffeList[FuNum],future,strlen(future));
          FuNum ++;
        }
        //9 12
      } else if(m2 < 12) {
        for(int i = 0; i<2; i++) {
          memset(future,'\0',31);
          sprintf(future,"%s%d%02d",it->first.c_str(),y1,(m1+i));
          memcpy(CffeList[FuNum],future,strlen(future));
          FuNum ++;
        }
        memset(future,'\0',31);
        sprintf(future,"%s%d%02d",it->first.c_str(),y1,12);
        memcpy(CffeList[FuNum],future,strlen(future));
        FuNum ++;
        memset(future,'\0',31);
        sprintf(future,"%s%d%02d",it->first.c_str(),y2,3);
        memcpy(CffeList[FuNum],future,strlen(future));
        FuNum ++;
        //12 3
      } else if(m2 < 15) {
        if(m1<=12) {
          memset(future,'\0',31);
          sprintf(future,"%s%d%02d",it->first.c_str(),y1,m1);
          memcpy(CffeList[FuNum],future,strlen(future));
          FuNum ++;
        } else {
          memset(future,'\0',31);
          sprintf(future,"%s%d%02d",it->first.c_str(),y2,m1-12);
          memcpy(CffeList[FuNum],future,strlen(future));
          FuNum ++;
        }

        if(m2<=12) {
          memset(future,'\0',31);
          sprintf(future,"%s%d%02d",it->first.c_str(),y1,m2);
          memcpy(CffeList[FuNum],future,strlen(future));
          FuNum ++;
        } else {
          memset(future,'\0',31);
          sprintf(future,"%s%d%02d",it->first.c_str(),y2,m2-12);
          memcpy(CffeList[FuNum],future,strlen(future));
          FuNum ++;
        }

        for(int i = 0; i<2; i++) {
          memset(future,'\0',31);
          sprintf(future,"%s%d%02d",it->first.c_str(),y2,(3+i*3));
          memcpy(CffeList[FuNum],future,strlen(future));
          FuNum ++;
        }

      } else if(m2 < 18) {
        //18 21
      }
    }

    if("TF" == it->first || "T" == it->first) {
      if(m1 <= 3) {
        for(int i = 0; i<3; i++) {
          memset(future,'\0',31);
          sprintf(future,"%s%d%02d",it->first.c_str(),y1,(3+i*3));
          memcpy(CffeList[FuNum],future,strlen(future));
          FuNum ++;
        }
        //3 6 9
      } else if(m1 <= 6) {
        for(int i = 0; i<3; i++) {
          memset(future,'\0',31);
          sprintf(future,"%s%d%02d",it->first.c_str(),y1,(6+i*3));
          memcpy(CffeList[FuNum],future,strlen(future));
          FuNum ++;
        }
        //6 9 12
      } else if(m1 <= 9) {
        memset(future,'\0',31);
        sprintf(future,"%s%d%02d",it->first.c_str(),y1,9);
        memcpy(CffeList[FuNum],future,strlen(future));
        FuNum ++;
        memset(future,'\0',31);
        sprintf(future,"%s%d%02d",it->first.c_str(),y1,12);
        memcpy(CffeList[FuNum],future,strlen(future));
        FuNum ++;
        memset(future,'\0',31);
        sprintf(future,"%s%d%02d",it->first.c_str(),y2,3);
        memcpy(CffeList[FuNum],future,strlen(future));
        FuNum ++;
        //9 12 3
      } else if(m1 <= 12) {
        memset(future,'\0',31);
        sprintf(future,"%s%d%02d",it->first.c_str(),y1,12);
        memcpy(CffeList[FuNum],future,strlen(future));
        FuNum ++;
        memset(future,'\0',31);
        sprintf(future,"%s%d%02d",it->first.c_str(),y2,3);
        memcpy(CffeList[FuNum],future,strlen(future));
        FuNum ++;
        memset(future,'\0',31);
        sprintf(future,"%s%d%02d",it->first.c_str(),y2,6);
        memcpy(CffeList[FuNum],future,strlen(future));
        FuNum ++;
        //12 3 6
      } else if(m1 <= 15) {
        memset(future,'\0',31);
        sprintf(future,"%s%d%02d",it->first.c_str(),y2,3);
        memcpy(CffeList[FuNum],future,strlen(future));
        FuNum ++;
        memset(future,'\0',31);
        sprintf(future,"%s%d%02d",it->first.c_str(),y2,6);
        memcpy(CffeList[FuNum],future,strlen(future));
        FuNum ++;
        memset(future,'\0',31);
        sprintf(future,"%s%d%02d",it->first.c_str(),y2,9);
        memcpy(CffeList[FuNum],future,strlen(future));
        FuNum ++;
        //15 18 21
      }
    }


  } //for
}

void FuList::ListInit(int type)
{
  int DifYear=2000;
  const std::map<std::string,std::string> *MP ;

  char (*pFL)[FUTRUE_ID_LEN];

  if(TYPE_CFFE == type) {
    pFL = CffeList ;
    MP = &M_CFFE;
  } else if(TYPE_SHFE == type) {
    pFL = ShfeList;
    MP = &M_SHFE;
  } else if(TYPE_CZCE == type) {
    DifYear = 2010;
    pFL = CzceList;
    MP = &M_CZCE;
  } else if(TYPE_DCE  == type) {
    pFL = DceList;
    MP = &M_DCE;
  } else {
    return;
  }

  int i;
  int m1,m2;
  int curYear;
  int MM[60] ;
  int FuNum = 0;

  char future[31];
  memset(future,'\0',31);

  for(auto it = (*MP).begin(); it != (*MP).end(); ++it) {
    ErrLog(1000,it->first.c_str(),1,0,0);
    ErrLog(1000,it->second.c_str(),1,0,0);

    if(1==isFirstDay(TYPE_CFFE,Year,Month,Day)) {
      m1 = Month + 1;
      m2 = Month + 2;
    } else {
      m1 = Month ;
      m2 = Month + 1;
    }
    // -------------begin---------------  各种异常处理 ---------------------
    if("AP" == it->first) {
      if(memcmp(Date,"20171222",8)<0) {
        continue;
      }
    }

    if("CY" == it->first) {
      if(memcmp(Date,"20170818",8)<0) {
        continue;
      }
    }

    // -- au ----------------------------------------
    if("au" == it->first) {
      int y1= Year -2000;
      int y2= y1+1;
      for(i=0; i<3; i++) {
        memset(future,'\0',31);
        m2 = m1 + i ;
        if(m2 > 12) {
          sprintf(future,"%s%d%02d",it->first.c_str(),y2,(m2-12));
          memcpy(pFL[FuNum],future,strlen(future));
          FuNum ++;
        } else {
          sprintf(future,"%s%d%02d",it->first.c_str(),y1,m2);
          memcpy(pFL[FuNum],future,strlen(future));
          FuNum ++;
        }
      }
      if(m1&1) {
        m2 = m1+3;
      } else {
        m2 = m1+4;
      }
      for(i=0; i<5; i++) {
        m2 += 2;
        if(m2 > 12) {
          sprintf(future,"%s%d%02d",it->first.c_str(),y2,(m2-12));
          memcpy(pFL[FuNum],future,strlen(future));
          FuNum ++;
        } else {
          sprintf(future,"%s%d%02d",it->first.c_str(),y1,m2);
          memcpy(pFL[FuNum],future,strlen(future));
          FuNum ++;
        }
      }
      continue;
    }
    //   -- bu ----------------------------------------
    if("bu" == it->first) {
      int y1= Year -2000;
      int y2= y1+1;
      for(i=0; i<6; i++) {
        memset(future,'\0',31);
        m2 = m1 + i ;
        if(m2 > 12) {
          sprintf(future,"%s%d%02d",it->first.c_str(),y2,(m2-12));
          memcpy(pFL[FuNum],future,strlen(future));
          FuNum ++;
        } else {
          sprintf(future,"%s%d%02d",it->first.c_str(),y1,m2);
          memcpy(pFL[FuNum],future,strlen(future));
          FuNum ++;
        }
      }
      m2 = ((m1+5)/3)*3;
      for(i=0; i<6; i++) {
        m2 += 3;
        if(m2 > 12) {
          int x = (m2-1)/12 ;
          sprintf(future,"%s%d%02d",it->first.c_str(),(y1+x),(m2-12*x));
          memcpy(pFL[FuNum],future,strlen(future));
          FuNum ++;
        } else {
          sprintf(future,"%s%d%02d",it->first.c_str(),y1,m2);
          memcpy(pFL[FuNum],future,strlen(future));
          FuNum ++;
        }
      }
      continue;
    }
    // -------------end ---------------  各种异常处理 ---------------------
    cJSON   *root ;
    cJSON   *jN ;
    cJSON   *jM ;
    cJSON   *tmp;
    int     Nn;      // "N" 的值··
    int     Mn;      // "M" 数组的长度 ..

    root = cJSON_Parse(it->second.c_str());

    jN = cJSON_GetObjectItem(root, "N");
    Nn = jN->valueint;

    jM = cJSON_GetObjectItem(root, "M");
    Mn = cJSON_GetArraySize(jM);

    // MM[]  1 3 5 7 9 11 13 15 17 19 21 23 25 ....
    // MM[]         ^ start          ^ end
    // ------  fill MM[60] ---------------------------------------------
    int tms = 60/Mn ;
    int lst = 60 - Mn*tms ;
    int k =0;
    for(int j =0; j< tms; j++) {
      for(i=0; i<Mn; i++) {
        tmp = cJSON_GetArrayItem(jM,i);
        MM[k] = tmp->valueint + j*12 ;
        k++;
      }
    }
    for(i=0; i<lst; i++) {
      tmp = cJSON_GetArrayItem(jM,i);
      MM[k] = tmp->valueint + tms*12;
      k++;
    }
    // ------- fill MM[2] ------ end -----------------------------------

    k=0;
    i=0;
    while(Month > MM[i]) {
      i++;
    }
    k=i;
    if(Month == MM[k]) {
      if(1 == isFirstDay(type,Year,Month,Day)) {
        k++;
      }
    }
    for(i=0; i<Nn; i++) {
      tms = (MM[k]-1)/12;
      curYear = Year + tms - DifYear;
      int curMonth = MM[k]-12*tms ;

      k++;
      memset(future,'\0',31);
      sprintf(future,"%s%d%02d",it->first.c_str(),curYear,curMonth);
      // ------------异常处理 过滤-----豆2-------------begin ---------
      if("b" == it->first && curYear <18) {
        if(curMonth&1) {
        } else {
          continue;
        }
      } else if("b" == it->first && curYear == 18) {
        if(curMonth ==2 ||  curMonth ==4) {
          continue;
        }
      }
      //  ------------异常处理 过滤------------------begin ---------
      //  苹果，新期货品种
      if(memcmp(future,"AP801",5)==0)  continue;
      if(memcmp(future,"AP803",5)==0)  continue;

      // 棉纱，新期货品种
      if(memcmp(future,"CY708",5)==0)  continue;
      if(memcmp(future,"CY709",5)==0)  continue;
      if(memcmp(future,"CY710",5)==0)  continue;
      if(memcmp(future,"CY711",5)==0)  continue;
      if(memcmp(future,"CY712",5)==0)  continue;

      // 燃料油 ==> 1 -12月（春节月份除外） (2016.2 2017.1   2018.2  2019.2 2020.1 )
      if(memcmp(future,"fu1602",6)==0)  continue;
      if(memcmp(future,"fu1701",6)==0)  continue;
      if(memcmp(future,"fu1802",6)==0)  continue;
      if(memcmp(future,"fu1902",6)==0)  continue;
      if(memcmp(future,"fu2001",6)==0)  continue;
      if(memcmp(future,"fu2102",6)==0)  continue;
      //  ------------异常处理 过滤------------------begin ---------
      memcpy(pFL[FuNum],future,strlen(future));
      FuNum ++;
    }

    cJSON_Delete(root);
  } // for 1

}

/*
  给成员变量 pShfeList pDceList  pCzceList pc_futures 赋值。
  FuList::pListInit() 需要在 FuList::Init() 执行后再执行
*/
void FuList::pListInit()
{
  int i;
  int m;
  int n = 0;
  for(i = 0; i< FUTURE_NUMBER; i++) {
    pc_futures[i] = nullptr;
  }
  for(i = 0; i< DCE_NUMBER; i++) {
    pShfeList[i] = nullptr;
    pDceList[i]  = nullptr;
    pCzceList[i] = nullptr;
  }
  for(i = 0; i< CFFE_NUMBER; i++) {
    pCffeList[i] = nullptr;
  }

  m = 0;
  for(i = 0; i< SHFE_NUMBER; i++) {
    if(strlen(ShfeList[i]) == 0) continue;
    pShfeList[m] = &ShfeList[i][0];
    m++;
    pc_futures[n] = &ShfeList[i][0];
    n++;
  }

  m = 0;
  for(i = 0; i< CZCE_NUMBER; i++) {
    if(strlen(CzceList[i]) == 0) continue;
    pCzceList[m] = &CzceList[i][0];
    m++;
    pc_futures[n] = &CzceList[i][0];
    n++;
  }

  m = 0;
  for(i = 0; i< DCE_NUMBER; i++) {
    if(strlen(DceList[i]) == 0) continue;
    pDceList[m] = &DceList[i][0];
    m++;
    pc_futures[n] = &DceList[i][0];
    n++;
  }

  m = 0;
  for(i = 0; i< CFFE_NUMBER; i++) {
    if(strlen(CffeList[i]) == 0) continue;
    pCffeList[m] = &CffeList[i][0];
    m++;
    pc_futures[n] = &CffeList[i][0];
    n++;
  }
}

} // namespace

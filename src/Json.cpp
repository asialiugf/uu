#include "uBEE.h"
#include <iostream>
#include <fstream>
#include <unistd.h>
#include <cjson/cJSON.h>
#include <string>
#include <vector>
#include <sys/time.h>
namespace uBEE
{

int Quotes(cJSON *quotes);
int Klines(cJSON *quotes);
int Ticks(cJSON *ticks);
int SaveError(cJSON *json, const char *msg);
std::string m_replace(std::string , const std::string &, const std::string &,int);
std::string QuotesReplace(std::string strSrc);
std::string KlinesReplace(std::string strSrc);
std::string KlinesCheck(const char * serials, cJSON *kline);
std::string TicksReplace(std::string strSrc);
std::string TicksCheck(const char * serials, cJSON *tick);

int Tqjson(const char*message,int tt)
{
  cJSON   *root ;
  cJSON   *json ;
  cJSON   *temp ;
  cJSON   *item ;
  cJSON   *data ;
  cJSON   *futu ;
  cJSON   *quotes ;
  cJSON   *klines ;
  cJSON   *ticks ;
  int   num=0;
  int   j=0;
  char *buf = nullptr;

  root = cJSON_Parse(message);
  if(!root) {
    ErrLog(1000,"message to json error!",1,0,0);
    return -1;
  }

  /*/
  buf = cJSON_Print(root);
  if(!buf) {
    ErrLog(1000,"---- buf = cJSON_Print(root) ---- error!",1,0,0);
    return -1;
  }
  SaveBin("newtick.json", buf, strlen(buf));
  SaveLine("newtick.json", "--------------- kkkkkkkkkkk ----------------------\n");
  free(buf);
  */

  temp = cJSON_GetObjectItem(root, "aid");
  if(!temp) {
    uBEE::ErrLog(1000,"no aid error!",1,0,0);
    return -1;
  }
  if(memcmp(temp->valuestring,"rtn_data",8)!=0) {
    uBEE::ErrLog(1000,"aid is not <rtn_data>!",1,0,0);
    return -1;
  }

  data = cJSON_GetObjectItem(root, "data");
  if(!data) {
    ErrLog(1000,"data error!",1,0,0);
  }

  num = cJSON_GetArraySize(data);
  if(num <=0) {
    return 0;
  }
  for(int i=0; i<num; i++) {
    temp = cJSON_GetArrayItem(data,i);
    if(!temp) {
      continue;
    }
    /*
      std::cout << "Please Input type: " << std::endl;
      std::cout << "1: Quotes only" << std::endl;
      std::cout << "2: Klines only" << std::endl;
      std::cout << "3: Ticks  only" << std::endl;
      std::cout << "4: Klines & Ticks  " << std::endl;
      std::cout << "5: Quotes & Kicks  " << std::endl;
      std::cout << "6: Quotes & Ticks  " << std::endl;
      std::cout << "7: All ==========  " << std::endl;
    */
    quotes = cJSON_GetObjectItem(temp, "quotes");
    if(quotes) {
      if(1==tt || 5==tt || 6==tt || 7==tt) {
        Quotes(quotes);
      }
    }
    klines = cJSON_GetObjectItem(temp, "klines");
    if(klines) {
      if(2==tt || 4==tt || 5==tt || 7==tt) {
        Klines(klines);
      }
    }
    ticks = cJSON_GetObjectItem(temp, "ticks");
    if(ticks) {
      if(3==tt || 4==tt || 6==tt || 7==tt) {
        Ticks(ticks);
      }
    }
  }
  cJSON_Delete(root);
  //uBEE::ErrLog(1000,"cJSON_Delete(root) !!!",1,0,0);
  return 0;
}


int Quotes(cJSON *quotes)
{
  char    ca_filename[512];
  int i=0;
  int num=0;
  cJSON *temp;
  cJSON *instrument_id;
  cJSON *futu;
  char * buf=NULL;
  num = cJSON_GetArraySize(quotes);
  if(num <=0) {
    uBEE::ErrLog(1000,"---- cJSON_GetArraySize(quotes); ----  error!",1,0,0);
    return -1;
  };
  for(i=0; i<num; i++) {
    temp = cJSON_GetArrayItem(quotes,i);
    if(!temp) {
      uBEE::ErrLog(1000,"---- cJSON_GetArrayItem(quotes,i) ---- error!",1,0,0);
      continue;
    }
    futu = cJSON_GetObjectItem(temp, "instrument_id");
    if(!futu) {
      uBEE::ErrLog(1000,"---- no instrument_id ---- error!",1,0,0);
      continue;
    }
    buf = cJSON_Print(temp);
    if(!buf) {
      uBEE::ErrLog(1000,"---- buf = cJSON_Print(temp) ---- error!",1,0,0);
      continue;
    }
    memset(ca_filename,'\0',512);
    sprintf(ca_filename,"../dat/%s",futu->valuestring);
    std::string mm = buf;
    std::string kk = QuotesReplace(mm);
    SaveBin(ca_filename, kk.c_str(), kk.length());

    free(buf);
  }
  return 0;
}
// --------------------------------------------------------------------- Klines ---------
int Klines(cJSON *klines)
{
  char    ca_filename[512];
  int i,j,k,l,ii,jj,kk,ll;

  cJSON *futu;
  cJSON *period;
  cJSON *data;
  cJSON *item;
  cJSON *datetime;

  char * buf=nullptr;
  char * pd=nullptr;
  char * js=nullptr;

  //uBEE::ErrLog(1000," enter into Klines()!",1,0,0);
  memset(ca_filename,'\0',512);
  ii = cJSON_GetArraySize(klines);
  if(ii<=0) {
    //SaveError(klines,"-K ----  ii = cJSON_GetArraySize(klines)  ---- error!\n");
    return -1;
  };

  for(i=0; i<ii; i++) {
    futu = cJSON_GetArrayItem(klines,i);  // get future 1,2,3...
    if(!futu) {
      //SaveError(klines,"-K ----  cJSON_GetArrayItem(klines,i) ---- error! ");
      continue;
    }

    jj = cJSON_GetArraySize(futu);
    if(jj <=0) {
      //SaveError(klines,"-K ---- jj = cJSON_GetArraySize(futu) ---- error!\n");
      continue;
    };
    for(j=0; j<jj; j++) {
      period = cJSON_GetArrayItem(futu,j);   // get period 3s 1m 5m ...
      if(!period) {
        //SaveError(klines,"-K ---- period = cJSON_GetArrayItem(futu,j) ---- error!\n");
        continue;
      }

      size_t len = strlen(period->string);
      if(len<6) {
        //SaveError(klines,"-K ---- period error  len<6---- error!\n");
        continue;
      }

      pd = period->string ;

      if(memcmp((char *)(pd+(len-6)),"000000",6)!=0) {
        uBEE::ErrLog(1000,pd,1,0,0);
        uBEE::ErrLog(1000,(char *)(pd+(len-6)),1,0,0);
        //SaveError(klines,"-K ---- period error ---- error!\n");
        continue;
      }
      *(pd+(len-9)) = '\0';

      kk = cJSON_GetArraySize(period);
      if(kk <=0) {
        //SaveError(klines,"-K ---- kk = cJSON_GetArraySize(period) ---- error!\n");
        continue;
      };
      for(k=0; k<kk; k++) {
        data = cJSON_GetArrayItem(period,k);  // get data from period ...
        if(!data) {
          //SaveError(klines,"-K ---- data ---- error!\n");
          continue;
        }

        ll = cJSON_GetArraySize(data);
        if(ll <=0) {
          //SaveError(klines,"-K---- ll = cJSON_GetArraySize(data) ----  error!\n");
          continue;
        };

        memset(ca_filename,'\0',512);
        sprintf(ca_filename,"../dat/%s.",futu->string);
        strcat(ca_filename,period->string);


        for(l=0; l<ll; l++) {
          item = cJSON_GetArrayItem(data,l);  // get item from data ...
          if(!item) {
            //SaveError(klines,"-K---- item error ---- error!\n");
            continue;
          }
          datetime = cJSON_GetObjectItem(item, "datetime");
          if(!datetime) {
            //SaveError(klines,"-K---- datetime ---- error!\n");
            continue;
          }


          // add +++++++++++++++++++++++++++++++++++++++++++++++++++
          std::string kline_msg = KlinesCheck(item->string, item);
          SaveLine(ca_filename,kline_msg.c_str());
          continue;
          // add +++++++++++++++++++++++++++++++++++++++++++++++++++

        } // l
      } // k
    } // j
  } // i
  return 0;
}

//------------------------------------------------------------------------------------------------
int Ticks(cJSON *ticks)
{

  char    ca_filename[512];
  int i,j,k,l,ii,jj,kk,ll;

  cJSON *futu;
  cJSON *period;
  cJSON *data;
  cJSON *item;
  cJSON *datetime;

  char * buf=NULL;
  char * pd=NULL;
  char * js=NULL;

  //uBEE::ErrLog(1000," enter into Klines()!",1,0,0);
  memset(ca_filename,'\0',512);
  ii = cJSON_GetArraySize(ticks);
  if(ii<=0) {
    //SaveError(ticks,"-T ----  ii = cJSON_GetArraySize(ticks)  ---- error!\n");
    return -1;
  };

  for(i=0; i<ii; i++) {
    futu = cJSON_GetArrayItem(ticks,i);  // get future 1,2,3...
    if(!futu) {
      //SaveError(ticks,"-T ----  cJSON_GetArrayItem(ticks,i) ---- error! ");
      continue;
    }

    kk = cJSON_GetArraySize(futu);
    if(kk <=0) {
      //SaveError(ticks,"-T ---- kk = cJSON_GetArraySize(period) ---- error!\n");
      continue;
    };

    memset(ca_filename,'\0',512);
    sprintf(ca_filename,"../dat/%s.tick",futu->string);

    for(k=0; k<kk; k++) {
      data = cJSON_GetArrayItem(futu,k);  // get data from future ...
      if(!data) {
        //SaveError(ticks,"-T ---- data ---- error!\n");
        continue;
      }

      ll = cJSON_GetArraySize(data);
      if(ll <=0) {
        //SaveError(ticks,"-T---- ll = cJSON_GetArraySize(data) ----  error!\n");
        continue;
      };


      for(l=0; l<ll; l++) {
        item = cJSON_GetArrayItem(data,l);  // get item from data ...
        if(!item) {
          //SaveError(ticks,"-T---- item error ---- error!\n");
          continue;
        }
        datetime = cJSON_GetObjectItem(item, "datetime");
        if(!datetime) {
          //SaveError(ticks,"-T---- datetime ---- error!\n");
          continue;
        }


        std::string tick_msg = TicksCheck(item->string, item);
        SaveLine(ca_filename,tick_msg.c_str());
        continue;

      } // l
    } // k
  } // i
  return 0;

}

std::vector<std::string> TicksCmd(const char *filename)
{

  std::ifstream file(filename);

  if(!file.is_open()) {
    std::cerr << "Unable to open file" << "\n";
    std::exit(-1);
  }

  std::vector<std::string> cmds;//this vector will be returned
  std::string line;
  std::string cmd;

  //"{\"chart_id\":\"VN_TA609\",\"aid\":\"set_chart\",\"duration\":0,\"view_width\":8000,\"ins_list\":\"TA609\"}");
  while(std::getline(file, line)) {
    cmd = "{\"chart_id\":\"VN_" + line + "_0\",\"aid\":\"set_chart\",\"duration\":0,\"view_width\":8000,\"ins_list\":\""
          + line + "\"}" ;
    cmds.push_back(cmd);
  }
  file.close();
  return cmds;
}

//---------------------------------------------------------------------------------------Command Kline ----
std::vector<std::string> KlinesCmd(const char *filename)
{
  std::ifstream file(filename);
  if(!file.is_open()) {
    std::cerr << "Unable to open file" << "\n";
    std::exit(-1);
  }

  std::vector<std::string> cmds;//this vector will be returned
  std::string line;
  std::string cmd;

  int period = 3;  //以秒来记

  // {"chart_id": "VN_c1701_3", "aid": "set_chart", "duration": 3000000000, "view_width": 8000, "ins_list": "c1701"}
  //                          3秒5秒10秒15秒20秒30秒1分2分 3分 5分 10分15分20分 30分 1小时2小时4小时，1日
  //std::vector<int> periods {3, 5, 10, 15, 20, 30, 60,120,180,300,600,900,1200,1800,3600,7200,14400};
  std::vector<int> periods {3, 5, 10, 15, 20, 30, 60,120,180,300,600,900};
  //int len = periods.size();

  std::string str1 = "{\"chart_id\":\"VN_" ;
  std::string str2 = "\",\"aid\":\"set_chart\",\"duration\":" ;
  std::string str3 = "000000000,\"view_width\":8964,\"ins_list\":\"" ;
  std::string str4 = "\"}" ;

  while(std::getline(file, line)) {
    for(auto iter = periods.cbegin(); iter != periods.cend(); iter++) {
      cmd = str1 + line + "_" + std::to_string(*iter) + str2 + std::to_string(*iter) + str3 + line + str4 ;
      cmds.push_back(cmd);
    }
  }
  file.close();
  return cmds;
}

//------------------------------------------------------------------------------------------------

int MkRequest(const char *filename, char *cmd, size_t cmd_len)
{
  FILE    *f = NULL;
  //char    *buf ;
  size_t   len = 0;
  size_t   iRc = 0;

  f = fopen(filename,"rb");
  if(f == NULL) {
    uBEE::ErrLog(1000,"cmd open error!",1,0,0);
    return errno ;
  }

  fseek(f, 0, SEEK_END);
  len = ftell(f);
  fseek(f, 0, SEEK_SET);
  if(len==0) {
    uBEE::ErrLog(1000,"cmd len  = 0  error!",1,0,0);
    return -2 ;
  }

  if(len > cmd_len) {
    uBEE::ErrLog(1000,"cmd buff is not enough!",1,0,0);
    return -1;
  }
  /*
  buf = (char*)malloc(len + 1);
  if(buf == NULL) {
    see_errlog(1000,"see_file_to_json(): Failed to allocate memory !!",RPT_TO_LOG,0,0) ;
    return errno ;
  }
  */
  iRc = fread(cmd, 1, len, f);
  cmd[len] = '\0';
  fclose(f);
  return 0;
}

std::string m_replace(std::string strSrc, const std::string &oldStr, const std::string &newStr,int count=-1)
{
  std::string strRet=strSrc;
  size_t pos = 0;
  int l_count=0;
  if(-1 == count) // replace all
    count = strRet.size();
  while((pos = strRet.find(oldStr, pos)) != std::string::npos) {
    strRet.replace(pos, oldStr.size(), newStr);
    if(++l_count >= count) break;
    pos += newStr.size();
  }
  return strRet;
}

std::string QuotesReplace(std::string strSrc)
{
  std::string mm=strSrc;
  std::string kk;
  kk = m_replace(mm,"\"last_price\"","\"E\"",-1);
  mm = m_replace(kk,"\"instrument_id\"","\"f\"",-1);
  kk = m_replace(mm,"\"open_interest\"","\"g\"",-1);
  mm = m_replace(kk,"\"pre_close\"","\"i\"",-1);
  kk = m_replace(mm,"\"pre_open_interest\"","\"j\"",-1);
  mm = m_replace(kk,"\"pre_settlement\"","\"k\"",-1);
  kk = m_replace(mm,"\"ask_price1\"","\"m\"",-1);
  mm = m_replace(kk,"\"ask_volume1\"","\"n\"",-1);
  kk = m_replace(mm,"\"bid_price1\"","\"p\"",-1);
  mm = m_replace(kk,"\"bid_volume1\"","\"q\"",-1);
  kk = m_replace(mm,"\"lower_limit\"","\"t\"",-1);
  mm = m_replace(kk,"\"upper_limit\"","\"u\"",-1);
  kk = m_replace(mm,"\"highest\"","\"H\"",-1);
  mm = m_replace(kk,"\"open\"","\"O\"",-1);
  kk = m_replace(mm,"\"close\"","\"C\"",-1);
  mm = m_replace(kk,"\"lowest\"","\"L\"",-1);
  kk = m_replace(mm,"\"settlement\"","\"s\"",-1);
  mm = m_replace(kk,"\"amount\"","\"a\"",-1);
  kk = m_replace(mm,"\"average\"","\"b\"",-1);
  mm = m_replace(kk,"\"datetime\"","\"d\"",-1);
  kk = m_replace(mm,"\"volume\"","\"V\"",-1);
  mm = m_replace(kk,"\"status\"","\"z\"",-1);
  kk = m_replace(mm,"\t","",-1);
  mm = m_replace(kk,"\n","",-1);
  return mm;
}
std::string KlinesReplace(std::string strSrc)
{
  std::string mm=strSrc;
  std::string kk;
  kk = m_replace(mm,"\"open_oi\"","\"g\"",-1);
  mm = m_replace(kk,"\"close_oi\"","\"j\"",-1);
  kk = m_replace(mm,"\"datetime\"","\"D\"",-1);
  mm = m_replace(kk,"\"open\"","\"O\"",-1);
  kk = m_replace(mm,"\"volume\"","\"V\"",-1);
  mm = m_replace(kk,"\"high\"","\"H\"",-1);
  kk = m_replace(mm,"\"close\"","\"C\"",-1);
  mm = m_replace(kk,"\"low\"","\"L\"",-1);
  kk = m_replace(mm,"\t","",-1);
  mm = m_replace(kk,"\n","",-1);
  return mm;
}
std::string TicksReplace(std::string strSrc)
{
  std::string mm=strSrc;
  std::string kk;
  kk = m_replace(mm,"\"ask_price1\"","\"m\"",-1);
  mm = m_replace(kk,"\"ask_volume1\"","\"n\"",-1);
  kk = m_replace(mm,"\"bid_volume1\"","\"q\"",-1);
  mm = m_replace(kk,"\"bid_price1\"","\"p\"",-1);
  kk = m_replace(mm,"\"last_price\"","\"E\"",-1);
  mm = m_replace(kk,"\"open_interest\"","\"g\"",-1);
  kk = m_replace(mm,"\"close\"","\"C\"",-1);
  mm = m_replace(kk,"\"datetime\"","\"D\"",-1);
  kk = m_replace(mm,"\"open\"","\"O\"",-1);
  mm = m_replace(kk,"\"highest\"","\"H\"",-1);
  kk = m_replace(mm,"\"lowest\"","\"L\"",-1);
  mm = m_replace(kk,"\"volume\"","\"V\"",-1);
  kk = m_replace(mm,"\"trading_day\"","\"d\"",-1);
  mm = m_replace(kk,"\t","",-1);
  kk = m_replace(mm,"\n","",-1);
  return kk;
}
// ----------------------------------------------------------------------Klines Check --------------
std::string KlinesCheck(const char * serials, cJSON *kline)
{
  cJSON *open_oi = cJSON_GetObjectItem(kline,"open_oi");
  cJSON *close_oi = cJSON_GetObjectItem(kline,"close_oi");
  cJSON *datetime = cJSON_GetObjectItem(kline,"datetime");
  cJSON *high = cJSON_GetObjectItem(kline,"high");
  cJSON *open = cJSON_GetObjectItem(kline,"open");
  cJSON *close = cJSON_GetObjectItem(kline,"close");
  cJSON *low = cJSON_GetObjectItem(kline,"low");
  cJSON *volume = cJSON_GetObjectItem(kline,"volume");

  double d_datetime = datetime->valuedouble;

  char ca_msg[1024] ;
  memset(ca_msg,'\0',1024);

  char ca_datetime[31];
  memset(ca_datetime,'\0',31);

  int ms=0;
  time_t lt;
  struct tm *newtime;
  double db = d_datetime/1000000000;

  lt =(time_t)db;
  newtime = localtime(&lt);
  strftime(ca_datetime, 31, "%F %T", newtime);

  sprintf(ca_msg,"D:%s S:%s H:%g O:%g C:%g L:%g OI:%g CI:%g V:%g",
          ca_datetime,
          serials,
          high->valuedouble,
          open->valuedouble,
          close->valuedouble,
          low->valuedouble,
          open_oi->valuedouble,
          close_oi->valuedouble,
          volume->valuedouble);
  std::string kk = ca_msg;
  return kk;
}


// ----------------------------------------------------------------------Ticks Check --------------
std::string TicksCheck(const char * serials, cJSON *tick)
{
  cJSON *ask_price1 = cJSON_GetObjectItem(tick,"ask_price1");
  cJSON *ask_volume1 = cJSON_GetObjectItem(tick,"ask_volume1");
  cJSON *bid_volume1 = cJSON_GetObjectItem(tick,"bid_volume1");
  cJSON *bid_price1 = cJSON_GetObjectItem(tick,"bid_price1");
  cJSON *last_price = cJSON_GetObjectItem(tick,"last_price");
  cJSON *open_interest = cJSON_GetObjectItem(tick,"open_interest");
  cJSON *datetime = cJSON_GetObjectItem(tick,"datetime");
  cJSON *highest = cJSON_GetObjectItem(tick,"highest");
  cJSON *lowest = cJSON_GetObjectItem(tick,"lowest");
  cJSON *volume = cJSON_GetObjectItem(tick,"volume");
  cJSON *trading_day = cJSON_GetObjectItem(tick,"trading_day");

  double d_datetime = datetime->valuedouble;
  double d_ActionDay = trading_day->valuedouble;

  char ca_msg[1024] ;
  memset(ca_msg,'\0',1024);

  char ca_datetime[31];
  memset(ca_datetime,'\0',31);

  char ca_trading_day[31];
  memset(ca_trading_day,'\0',31);

  int ms=0;
  time_t lt;
  struct tm *newtime;
  double db = d_datetime/1000000000;

  // ------------------------------ mili sec -------------
  char ca_dt[100];
  char ca_UpdateMillisec[7];
  memset(ca_dt,'\0',100);
  memset(ca_UpdateMillisec,'\0',7);
  sprintf(ca_dt,"%lf",db);
  int len = strlen(ca_dt);
  int len2 = len-6;
  memcpy(ca_UpdateMillisec,&ca_dt[len2],6);

  lt =(time_t)db;
  newtime = localtime(&lt);
  strftime(ca_datetime, 31, "%F %T", newtime);

  char ca_TradingDay[9] ;
  memset(ca_TradingDay,'\0',9);
  memcpy(&ca_TradingDay[0],&ca_datetime[0],4);
  memcpy(&ca_TradingDay[4],&ca_datetime[5],2);
  memcpy(&ca_TradingDay[6],&ca_datetime[8],2);

  char ca_UpdateTime[9] ;
  memset(ca_UpdateTime,'\0',9);
  memcpy(&ca_UpdateTime[0],&ca_datetime[11],8);

  db = d_ActionDay/1000000000;
  lt =(time_t)db;
  newtime = localtime(&lt);
  strftime(ca_trading_day, 31, "%F", newtime);

  char ca_ActionDay[9] ;
  memset(ca_ActionDay,'\0',9);
  memcpy(&ca_ActionDay[0],&ca_trading_day[0],4);
  memcpy(&ca_ActionDay[4],&ca_trading_day[5],2);
  memcpy(&ca_ActionDay[6],&ca_trading_day[8],2);
  

  sprintf(ca_msg,"A:%s %s %s S:%s T:%s H:%g L:%g LP:%g AP:%g AV:%g BP:%g BV:%g OI:%g V:%g",
          ca_TradingDay,
          ca_UpdateTime,
          ca_UpdateMillisec,
          serials,
          ca_ActionDay,
          highest->valuedouble,
          lowest->valuedouble,
          last_price->valuedouble,
          ask_price1->valuedouble,
          ask_volume1->valuedouble,
          bid_price1->valuedouble,
          bid_volume1->valuedouble,
          open_interest->valuedouble,
          volume->valuedouble);
  std::string kk = ca_msg;
  return kk;
}


int SaveError(cJSON *json, const char *msg)
{
  char ca_filename[512];
  char * js= cJSON_Print(json);
  memset(ca_filename,'\0',512);
  sprintf(ca_filename,"../dat/%s",json->string);
  uBEE::ErrLog(1000,msg,1,0,0);
  SaveLine(ca_filename, msg);
  SaveBin(ca_filename, js, strlen(js));
  free(js);
  return 0;
}

} // namespace uBEE

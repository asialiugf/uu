#include <uWS/uWS.h>
#include "uBEE.h"
//#include "Psqlpool.h"
//#include "PsqlFunc.h"
#include <iostream>
#include <sys/time.h>

#pragma comment(lib, "uWS.lib")

/*
void createTable(std::shared_ptr<uBEE::DBPool> dbpool,const char * future);
*/
int main()
{
  uWS::Hub h;

  int tt = 0 ;
  int ft = 1 ;
  int y,m,d;
  char filename[256];

  std::cout << std::endl;
  std::cout << "            Input type description: " << std::endl;
  std::cout << "            1: Quotes only" << std::endl;
  std::cout << "            2: Klines only" << std::endl;
  std::cout << "            3: Ticks  only" << std::endl;
  std::cout << "            4: Klines & Ticks  " << std::endl;
  std::cout << "            5: Quotes & Kicks  " << std::endl;
  std::cout << "            6: Quotes & Ticks  " << std::endl;
  std::cout << "            7: All  Q & K & T  " << std::endl;
  std::cout << "            Please Input type: ";
  std::cin >> tt ;
  std::cout << std::endl;
  std::cout << "            X: Pleae Input Year:  ";
  std::cin >> y;
  std::cout << std::endl;
  std::cout << "            X: Pleae Input Month:  ";
  std::cin >> m ;
  std::cout << std::endl;
  std::cout << "            X: Pleae Input Day:  ";
  std::cin >> d ;
  std::cout << std::endl;

  uBEE::FuList fl;
  int rtn = fl.Init(y,m,d);
  if(rtn < 0) {
    std::cout << " 期货列表 生成 失败！ " << std::endl;
    exit(-1);
  }

  if(6==fl.Week || 7==fl.Week) {
    std::cout << "week:"<< fl.Week << "    date:"<< fl.Date << std::endl;
    std::cout << "这一天不是交易日, not trading day, week: " << fl.Week << std::endl;
    return -1;
  }

  std::cout << "week:"<< fl.Week << "    date:"<< fl.Date << std::endl;

  std::cout << std::endl;
  std::cout << "            Input type description: " << std::endl;
  std::cout << "            --- [0]: CFFE  --- [1]: SHFE  --- [2]: CZCE  --- [3]: DCE  ---" << std::endl;
  std::cout << "            7: All  CFFE & SHFE & CZCE & DCE  " << std::endl;
  std::cout << "            Please Input type: ";
  std::cin >> ft ;
  std::cout << std::endl;

  usleep(2000000);

  // 创建数据库连接池
  //auto dbpool = std::make_shared<uBEE::DBPool>();


  memset(filename,'\0',256);
  sprintf(filename,"%s",fl.Date);
  std::cout << " \n---------------------------------------------------------------------------\n";
  char fn_shfe[256];
  memset(fn_shfe,'\0',256);
  sprintf(fn_shfe,"%s.shfe",fl.Date);
  for(int i = 0; i< SHFE_NUMBER; i++) {
    std::cout << fl.ShfeList[i] << " " ;
    if(strlen(fl.ShfeList[i]) == 0) continue;
    uBEE::SaveLine(fn_shfe,fl.ShfeList[i]);
    //createTable(dbpool,fl.ShfeList[i]);
    //uBEE::createTickTable(dbpool,fl.ShfeList[i]);
    if(ft==7) {
      uBEE::SaveLine(filename,fl.ShfeList[i]);
    }
    if(memcmp(fl.ShfeList[i],fl.ShfeList[i+1],2)!=0) {
      std::cout << std::endl;
    }
  }
  std::cout << " \n---------------------------------------------------------------------------\n";
  char fn_czce[256];
  memset(fn_czce,'\0',256);
  sprintf(fn_czce,"%s.czce",fl.Date);
  for(int i = 0; i< CZCE_NUMBER; i++) {
    std::cout << fl.CzceList[i] <<  " " ;
    if(strlen(fl.CzceList[i]) == 0) continue;
    uBEE::SaveLine(fn_czce,fl.CzceList[i]);
    //createTable(dbpool,fl.CzceList[i]);
   // uBEE::createTickTable(dbpool,fl.CzceList[i]);
    if(ft==7) {
      uBEE::SaveLine(filename,fl.CzceList[i]);
    }
    if(memcmp(fl.CzceList[i],fl.CzceList[i+1],2)!=0) {
      std::cout << std::endl;
    }
  }
  std::cout << " \n---------------------------------------------------------------------------\n";
  char fn_dce[256];
  memset(fn_dce,'\0',256);
  sprintf(fn_dce,"%s.dce",fl.Date);
  for(int i = 0; i< DCE_NUMBER; i++) {
    std::cout << fl.DceList[i] << " " ;
    if(strlen(fl.DceList[i]) == 0) continue;
    uBEE::SaveLine(fn_dce,fl.DceList[i]);
    //createTable(dbpool,fl.DceList[i]);
    // uBEE::createTickTable(dbpool,fl.DceList[i]);
    if(ft==7) {
      uBEE::SaveLine(filename,fl.DceList[i]);
    }
    if(memcmp(fl.DceList[i],fl.DceList[i+1],2)!=0) {
      std::cout << std::endl;
    }
  }
  std::cout << " \n---------------------------------------------------------------------------\n";
  char fn_cffe[256];
  memset(fn_cffe,'\0',256);
  sprintf(fn_cffe,"%s.cffe",fl.Date);
  for(int i = 0; i< CFFE_NUMBER; i++) {
    std::cout << fl.CffeList[i] << " " ;
    if(strlen(fl.CffeList[i]) == 0) continue;
    uBEE::SaveLine(fn_cffe,fl.CffeList[i]);
    //createTable(dbpool,fl.CffeList[i]);
    // uBEE::createTickTable(dbpool,fl.CffeList[i]);
    if(ft==7) {
      uBEE::SaveLine(filename,fl.CffeList[i]);
    }
    if(memcmp(fl.CffeList[i],fl.CffeList[i+1],2)!=0) {
      std::cout << std::endl;
    }
  }
  std::cout << " \n---------------------------------------------------------------------------\n";


  // --------------------------------------------------------------------------------------------
  // 服务端接收到包后原封不动返回
  /*
  h.onConnection([&tt](uWS::WebSocket<uWS::SERVER> *ws, uWS::HttpRequest req) {
    if(tt != 1) {
      std::vector<std::string> cmds = uBEE::Command("./cmd");
      for_each(cmds.cbegin(), cmds.cend(), [&ws](const std::string &request)->void{
        ws->send(request.c_str());
        std::cout << request << std::endl;
      });
    }
    std::cout <<"Server onConnection send: --server--" << std::endl;
  });

  h.onMessage([&tt](uWS::WebSocket<uWS::SERVER> *ws, char *message, size_t length, uWS::OpCode opCode) {
    if(length>0) {
      std::cout << "message!" << std::endl;
      //usleep(5000000);
      //message[length] = 0;
      struct  timeval start;
      struct  timeval end1;
      unsigned  long diff;
      gettimeofday(&start,NULL);
      uBEE::Tqjson(message,tt);
      gettimeofday(&end1,NULL);
      diff = 1000000 * (end1.tv_sec-start.tv_sec)+ end1.tv_usec-start.tv_usec;
      printf("thedifference is %ld\n",diff);
    }
  });
  h.onPong([](uWS::WebSocket<uWS::SERVER> *ws, char *message, size_t length) {
    std::cout<< "ok ping\n";
  });
  h.getDefaultGroup<uWS::SERVER>().startAutoPing(2000);

  //bool k = h.listen(3000) ;
  //if(!k) {
  //  std::cout << " listen error !!" << std::endl;
  //}
  */
  // --------------------------------------------------------------------------------------------
  // 客户端

  h.onDisconnection([](uWS::WebSocket<uWS::CLIENT> *ws, int code, char *message, size_t length) {
    std::cout << "onDisconnection !!" << std::endl;
    std::cout << message << std::endl;
  });


  h.onConnection([&tt,&ft,&fn_shfe,&fn_czce,&fn_dce,&fn_cffe,&filename](uWS::WebSocket<uWS::CLIENT> *ws, uWS::HttpRequest req) {
    std::cout <<"Tqdata.x onConnection !!" << std::endl;
    std::vector<std::string> cmds ;

    if(2==tt || 4==tt || 5==tt || 7==tt) {
      switch(ft) {
      case(0):
        cmds = uBEE::KlinesCmd(fn_cffe);
        break;
      case(1):
        cmds = uBEE::KlinesCmd(fn_shfe);
        break;
      case(2):
        cmds = uBEE::KlinesCmd(fn_czce);
        break;
      case(3):
        cmds = uBEE::KlinesCmd(fn_dce);
        break;
      case(7):
        cmds = uBEE::KlinesCmd(filename);
        break;
      }
      int count = cmds.size();
      for(int i = 0; i<count; i++) {
        std::cout << cmds[i] << std::endl;
        ws->send(cmds[i].c_str());
      }
    }
    if(3==tt || 4==tt || 6==tt || 7==tt) {
      switch(ft) {
      case(0):
        cmds = uBEE::TicksCmd(fn_cffe);
        break;
      case(1):
        cmds = uBEE::TicksCmd(fn_shfe);
        break;
      case(2):
        cmds = uBEE::TicksCmd(fn_czce);
        break;
      case(3):
        cmds = uBEE::TicksCmd(fn_dce);
        break;
      case(7):
        cmds = uBEE::TicksCmd(filename);
        break;
      }
      int count = cmds.size();
      //for(int i = 0; i <200; i++) {
      for(int i = 0; i<count; i++) {
        std::cout << cmds[i] << std::endl;
        ws->send(cmds[i].c_str());
      }
    }
    /*
    " --- [0]: CFFE  --- [1]: SHFE  --- [2]: CZCE  --- [3]: DCE  ---"
    for_each(cmds.cbegin(), cmds.cend(), [&ws](const std::string &request)->void{
      ws->send(request.c_str());
      std::cout << request << std::endl;
    });
    */
  });

  h.onError([](void *user) {
    switch((long) user) {
    case 1:
      std::cout << "login error!!" << std::endl;
      exit(-1);
    default:
      std::cout << "FAILURE: " << user << " should not emit error!" << std::endl;
      exit(-1);
    }
  });

  h.onMessage([&](uWS::WebSocket<uWS::CLIENT> *ws, char *message, size_t length, uWS::OpCode opCode) {
    if(length>0) {
      std::cout << "message!" << std::endl;
      //usleep(5000000);
      //message[length] = 0;
      struct  timeval start;
      struct  timeval end1;
      unsigned  long diff;
      //uBEE::SaveBin("../tick/tick.json",(const char*)message,length);
      gettimeofday(&start,NULL);
      uBEE::Tqjson(message,tt);
      gettimeofday(&end1,NULL);
      diff = 1000000 * (end1.tv_sec-start.tv_sec)+ end1.tv_usec-start.tv_usec;
      printf("thedifference is %ld\n",diff);
    }
  });

  h.onPong([](uWS::WebSocket<uWS::CLIENT> *ws, char *message, size_t length) {
    std::cout<< "ping\n";
  });
  h.getDefaultGroup<uWS::CLIENT>().startAutoPing(2000);

  //h.connect("ws://192.168.3.7:7777",(void *) 1);
  h.connect("ws://192.168.3.210:7777",(void *) 1);
// --------------------------------------------------------------------------------------------

  h.run();
}


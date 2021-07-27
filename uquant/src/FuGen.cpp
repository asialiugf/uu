#include "uBEE.h"
//#include <Base.h>
#include <thread>
#include <unistd.h>
#include <iostream>

int main()
{
  int y;
  int m;
  int d;
  std::cout << "请输入日期:" << std::endl;
  std::cout << "请输入year:" << std::endl;
  std::cin >> y;
  std::cout << "请输入month:" << std::endl;
  std::cin >> m;
  std::cout << "请输入day:" << std::endl;
  std::cin >> d;
  // ------------------------------------------------------------
  uBEE::FuList fl;
  int rtn = fl.Init(y,m,d);
  if(rtn < 0) {
    exit (-1);
  }
  //fl.Init("20170418");
  std::cout << "week:"<< fl.Week << std::endl;
  std::cout << "date:"<< fl.Date << std::endl;
  std::cout << "date:"<< fl.Year << std::endl;
  std::cout << "date:"<< fl.Month << std::endl;
  std::cout << "date:"<< fl.Day << std::endl;
  std::cout << " \n---------------------------------------------------------------------------\n";
  for(int i = 0; i< SHFE_NUMBER; i++) {
    std::cout << fl.ShfeList[i] << " " ;
    if(memcmp(fl.ShfeList[i],fl.ShfeList[i+1],2)!=0) {
       std::cout << std::endl;
    }
  }
  std::cout << " \n---------------------------------------------------------------------------\n";
  for(int i = 0; i< SHFE_NUMBER; i++) {
    std::cout << fl.CzceList[i] <<  " " ;
    if(memcmp(fl.CzceList[i],fl.CzceList[i+1],2)!=0) {
       std::cout << std::endl;
    }
  }
  std::cout << " \n---------------------------------------------------------------------------\n";
  for(int i = 0; i< SHFE_NUMBER; i++) {
    std::cout << fl.DceList[i] << " " ;
    if(memcmp(fl.DceList[i],fl.DceList[i+1],2)!=0) {
       std::cout << std::endl;
    }
  }
  std::cout << " \n---------------------------------------------------------------------------\n";
  for(int i = 0; i< CFFE_NUMBER; i++) {
    std::cout << fl.CffeList[i] << " " ;
    if(memcmp(fl.CffeList[i],fl.CffeList[i+1],2)!=0) {
       std::cout << std::endl;
    }
  }
  std::cout << " \n---------------------------------------------------------------------------\n";
  std::cout << fl.Date << "????" << std::endl;
  return 0;
}

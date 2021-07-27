#include "Global.h"
#include "uBEE.h"
#include "MdCtp.h"
#include "MkSim.h"
#include "Bars.h"
#include <thread>
#include <unistd.h>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/prctl.h>

extern char **environ;
namespace uBEE
{
} // end namespace
int ForkApi();
int ForkBck();
int ForkCtp();
int ForkSim();
int Menu();
uBEE::HubBck hb;
using namespace uBEE ;

int main(int argc, char **argv)
{

  int rtn;
  int pid;

  int iFunc = Menu();

  uBEE::Daemon(1,0) ;

  // --- time block init !! ------------ from Global.h
  uBEE::InitAll();
  /* ----------test  */
  std::cout << "uuuuuuuuuuuuuuuuuuuuu\n";
  for(int j=0; j<7; j++) {
    int i = 0;
    while(i<SGM_NUM &&tb->TT[j].aSgms[i].iI !=-1) {
      std::cout << "----:"<< tb->TT[j].aSgms[i].cB ;
      std::cout << "----:"<< tb->TT[j].aSgms[i].cE ;
      std::cout << "----:"<< tb->TT[j].aSgms[i].iB ;
      std::cout << "----:"<< tb->TT[j].aSgms[i].iE ;
      std::cout << "----:"<< tb->TT[j].aSgms[i].iI << std::endl;
      i++;
    }
    std::cout << std::endl;
  }
  std::cout << "uuuuuuuuuuuuuuuuuuuuu\n";
  /*---------- test---------- */

  hb.Init();
  hb.Start();

  pid = getpid();

  uBEE::SaveArgv(argc,argv);
  uBEE::InitSetProcTitle();
  uBEE:: SetProcTitle("master ","DataServ: ");


  std::cout << "befor fork api !!\n" ;
  rtn = ForkApi();
  sleep(1);
  std::cout << "after fork api !!\n" ;

  if(iFunc == 2 || iFunc == 6 || iFunc == 7 || iFunc == 8) {
    for(int i=0; i<5; i++) {
      rtn = ForkBck();
      sleep(1);
    }
  }
  if(iFunc == 4 || iFunc == 5 || iFunc == 7 || iFunc == 8) {
    rtn = ForkSim();
    sleep(1);
  }
  if(iFunc == 3 || iFunc == 5 || iFunc == 6 || iFunc == 8) {
    rtn = ForkCtp();
    sleep(1);
  }

  if( iFunc<2 || iFunc>8 ) {
    std::cout << "Input Error !! \n" ;
    exit(-1);
  }

  while(true) {
    sleep(1000);
  }

}

int ForkApi()
{
  int rc;
  int pid;
  int i = 0;
  char ca_futures[1024];
  pid = fork();
  uBEE::HubApi hub;
  switch(pid) {
  case -1:
    return -1;

  case 0:
    pid = getpid();
    uBEE::InitSetProcTitle();
    uBEE:: SetProcTitle("HubApi","DataServ: ");
    hub.Init();
    hub.Start();
    break;
  default:
    break;
  }
  return 0;
}

int ForkBck()
{
  int rc;
  int pid;
  int i = 0;
  pid = fork();
  switch(pid) {
  case -1:
    return -1;

  case 0:
    pid = getpid();
    uBEE::InitSetProcTitle();
    uBEE:: SetProcTitle("HubBck:","DataServ: ");
    hb.Run();
    break;
  default:
    break;
  }
  return 0;
}

int ForkCtp()
{
  int rc;
  int pid;
  pid = fork();

  switch(pid) {
  case -1:
    return -1;
  case 0: {

    pid = getpid();
    uBEE::InitSetProcTitle();
    uBEE::SetProcTitle("HubCtp:","DataServ: ");

    uBEE::HubCtp hub;
    hub.Init();
    sleep(2);

    std::thread t([&hub] {
      uBEE::MdCtp(hub.sg);
    });  /* thread t */
    t.detach();
    hub.Start();
    std::cout << "end hub.Start!!" << std::endl;
  }
  break;
  default:
    break;
  }
  return 0;
}

int ForkSim()
{
  int rc;
  int pid;
  int i = 0;
  pid = fork();
  switch(pid) {
  case -1:
    return -1;

  case 0: {
    pid = getpid();
    uBEE::InitSetProcTitle();
    uBEE:: SetProcTitle("HubSim:","DataServ: ");

    uBEE::HubSim hub;
    hub.Init();
    sleep(2);

    //------ 开一个新的线程----------------
    std::thread t([&hub] {
      uBEE::MkSim(hub.sg);
    });  /* thread t */
    t.detach();
    hub.Start();
    std::cout << "end hub.Start!!" << std::endl;

  }
  break;
  default:
    break;
  }
  return 0;
}

int Menu()
{
  int tt;
  std::cout << std::endl;
  std::cout << "            \033[31m Input type description: \033[0m" << std::endl;
  std::cout << "            \033[35m 2: \033[35mBck\033[0m only\033[0m" << std::endl;
  std::cout << "            \033[31m 3: \033[31mCtp\033[0m only\033[0m" << std::endl;
  std::cout << "            \033[32m 4: \033[32mSim\033[0m only\033[0m" << std::endl;
  std::cout << "            \033[31m 5: \033[31mCtp\033[0m + \033[32mSim\033[0m" << std::endl;
  std::cout << "            \033[31m 6: \033[31mCtp\033[0m + \033[35mBck\033[0m" << std::endl;
  std::cout << "            \033[32m 7: \033[32mSim\033[0m + \033[35mBck\033[0m" << std::endl;
  std::cout << "            \033[31m 8: \033[31mCtp\033[0m + \033[32mSim\033[0m + \033[35mBck\033[0m" << std::endl;
  std::cout << "            \033[31m Please Input type: \033[0m";
  std::cin >> tt ;
  return tt;
}

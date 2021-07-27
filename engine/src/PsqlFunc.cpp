#include <iostream>
#include <thread>
#include <fstream>
#include <sstream>
#include "Psqlpool.h"
#include "PsqlFunc.h"
#include <iostream>
#include <string.h>

namespace uBEE
{


void createBarTable(std::shared_ptr<uBEE::DBPool> dbpool,const char * future, const char * period)
{
  auto conn = dbpool->getDBConn();
  PGresult   *res;
  PGconn     *connx;
  connx = conn->getConn().get() ;

  char       ca_state[4096];
  memset(ca_state,'\0',4096);

  const unsigned char strSrc[]="\
  datetime        VARCHAR(10) NOT NULL,\
  serials         VARCHAR UNIQUE NOT NULL,\
  high            DOUBLE PRECISION NOT NULL,\
  open            DOUBLE PRECISION NOT NULL,\
  close		      DOUBLE PRECISION NOT NULL,\
  low		      DOUBLE PRECISION NOT NULL,\
  open_oi	      DOUBLE PRECISION NOT NULL,\
  close_oi        DOUBLE PRECISION NOT NULL,\
  volume          DOUBLE PRECISION NOT NULL,\
  PRIMARY KEY(datetime)\
  ";

  sprintf(ca_state,"CREATE TABLE BAR_%s_%s(%s); ",future,period,strSrc);
  std::cout << ca_state << std::endl;

  res = PQexec(connx, ca_state);
  if(PQresultStatus(res) != PGRES_COMMAND_OK) {
    fprintf(stderr, " BEGIN command failed: %s", PQerrorMessage(connx));
    PQclear(res);
    //exit_nicely(connx);
    PQfinish(connx);
    exit(1);
  }
  PQclear(res);

  dbpool->freeDBConn(conn);
}

void createTickTable(std::shared_ptr<uBEE::DBPool> dbpool,const char * future)
{
  auto conn = dbpool->getDBConn();
  PGresult   *res;
  PGconn     *connx;
  connx = conn->getConn().get() ;

  char       ca_state[4096];
  memset(ca_state,'\0',4096);

  const unsigned char strSrc[]="\
  TradingDay        VARCHAR(9) NOT NULL,\
  UpdateTime        VARCHAR(9) NOT NULL,\
  UpdateMillisec    VARCHAR(10) NOT NULL,\
  ActionDay         VARCHAR(10),\
  serials         VARCHAR UNIQUE NOT NULL,\
  HighestPrice         DOUBLE PRECISION NOT NULL,\
  LowestPrice          DOUBLE PRECISION NOT NULL,\
  LastPrice      DOUBLE PRECISION NOT NULL,\
  AskPrice1      DOUBLE PRECISION NOT NULL,\
  AskVolume1     DOUBLE PRECISION NOT NULL,\
  BidPrice1      DOUBLE PRECISION NOT NULL,\
  BidVolume1     DOUBLE PRECISION NOT NULL,\
  OpenInterest   DOUBLE PRECISION NOT NULL,\
  Volume          DOUBLE PRECISION NOT NULL,\
  PRIMARY KEY(TradingDay,UpdateTime,ms)\
  ";
  sprintf(ca_state,"CREATE TABLE TICK_%s(%s); ",future,strSrc);
  std::cout << ca_state << std::endl;

  /*
  res = PQexec(connx, ca_state);
  if(PQresultStatus(res) != PGRES_COMMAND_OK) {
    fprintf(stderr, "BEGIN command failed: %s", PQerrorMessage(connx));
    PQclear(res);
    //exit_nicely(connx);
    PQfinish(connx);
    exit(1);
  }
  PQclear(res);
  */
  dbpool->freeDBConn(conn);
}

} //end namesapce

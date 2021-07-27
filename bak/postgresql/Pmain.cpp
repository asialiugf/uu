#include <thread>
#include <iostream>
#include "Psqlpool.h"
#include <string.h>


static void
exit_nicely(PGconn *conn)
{
  PQfinish(conn);
  exit(1);
}

void testConnection(std::shared_ptr<DBPool> dbpool)
{
  auto conn = dbpool->getDBConn();

  //std::string demo = "SELECT max(id) FROM demo; " ;
  const char* demo = "SELECT max(id) FROM demo; " ;
  PQsendQuery(conn->getConn().get(), demo);
  //PQsendQuery( conn->connection().get(), demo.c_str() );

  while(auto res_ = PQgetResult(conn->getConn().get())) {
    if(PQresultStatus(res_) == PGRES_TUPLES_OK && PQntuples(res_)) {
      auto ID = PQgetvalue(res_ ,0, 0);
      std::cout<< ID<<std::endl;
    }

    if(PQresultStatus(res_) == PGRES_FATAL_ERROR) {
      std::cout<< PQresultErrorMessage(res_)<<std::endl;
    }

    PQclear(res_);
  }

  dbpool->freeDBConn(conn);

}


int main(int argc, char const *argv[])
{

  auto dbpool = std::make_shared<DBPool>();
  std::vector<std::shared_ptr<std::thread>> vec;

  for(size_t i = 0; i< 50 ; ++i) {

    vec.push_back(std::make_shared<std::thread>(std::thread(testConnection, dbpool)));
  }

  for(auto &i : vec) {
    i.get()->join();
  }

  auto conn = dbpool->getDBConn();
  PGresult   *res;
  PGconn     *connx;
  connx = conn->getConn().get() ;

  char       ca_state[1024];
  memset(ca_state,'\0',1024);

  sprintf(ca_state,"CREATE TABLE haha5(%s int4)","test1");

  res = PQexec(connx, ca_state);
  if(PQresultStatus(res) != PGRES_COMMAND_OK) {
    fprintf(stderr, "BEGIN command failed: %s", PQerrorMessage(connx));
    PQclear(res);
    exit_nicely(connx);
  }
  PQclear(res);

  dbpool->freeDBConn(conn);

  return 0;
}

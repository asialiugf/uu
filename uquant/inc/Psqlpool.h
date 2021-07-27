#ifndef PGBACKEND_H
#define PGBACKEND_H

#include <memory>
#include <mutex>
#include <string>
#include <queue>
#include <condition_variable>
#include <libpq-fe.h>
//#include "pgconnection.h"

namespace uBEE
{

class DBConn
{
public:
    DBConn();
    std::shared_ptr<PGconn> getConn() const;

private:
    //void establish_connection();
    const char* m_dbhost = "localhost";
    const char* m_dbport = "5432";
    const char* m_dbname = "demo";
    const char* m_dbuser = "postgres";
    const char* m_dbpass = "postgres";
    std::shared_ptr<PGconn>  m_connection;

};

class DBPool
{
public:
    DBPool();
    std::shared_ptr<DBConn> getDBConn();
    void freeDBConn(std::shared_ptr<DBConn>);

private:
    void createPool();
    
    std::mutex m_mutex;
    std::condition_variable m_condition;
    std::queue<std::shared_ptr<DBConn>> m_pool;

    const int POOL = 10;

};

} //end namesapce
#endif //PGBACKEND_H

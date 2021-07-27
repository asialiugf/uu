
#include <iostream>
#include <thread>
#include <fstream>
#include <sstream>
#include "Psqlpool.h"

DBConn::DBConn()
{
  m_connection.reset(PQsetdbLogin(m_dbhost, m_dbport, nullptr, nullptr,m_dbname, m_dbuser, m_dbpass), &PQfinish);

  if(PQstatus(m_connection.get()) != CONNECTION_OK && PQsetnonblocking(m_connection.get(), 1) != 0) {
    throw std::runtime_error(PQerrorMessage(m_connection.get()));
  }

}


std::shared_ptr<PGconn> DBConn::getConn() const
{
  return m_connection;
}

DBPool::DBPool()
{
  createPool();
}

void DBPool::createPool()
{
  std::lock_guard<std::mutex> locker_(m_mutex);

  for(auto i = 0; i< POOL; ++i) {
    m_pool.emplace(std::make_shared<DBConn>());
  }
}

std::shared_ptr<DBConn> DBPool::getDBConn()
{

  std::unique_lock<std::mutex> lock_(m_mutex);

  while(m_pool.empty()) {
    m_condition.wait(lock_);
  }

  auto conn_ = m_pool.front();
  m_pool.pop();

  return  conn_;
}


void DBPool::freeDBConn(std::shared_ptr<DBConn> conn_)
{
  std::unique_lock<std::mutex> lock_(m_mutex);
  m_pool.push(conn_);
  lock_.unlock();
  m_condition.notify_one();
}

#ifndef UBEE_DAEMON_H
#define UBEE_DAEMON_H

#include <uWS/uWS.h>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <functional>
#include <vector>
#include <queue>

namespace uBEE
{
int Daemon(int nochdir,int noclose);
}

#endif // UBEE_DAEMON_H

#ifndef NET_EVENTLOOPTHREAD_H
#define NET_EVENTLOOPTHREAD_H

#include "Condition.h"
#include "Mutex.h"
#include "Thread.h"
#include "Noncopyable.h"

namespace fnet
{

class EventLoop;

class EventLoopThread : public Noncopyable
{
public:
  EventLoopThread();
  ~EventLoopThread();
  EventLoop* startLoop();

private:
  void threadFunc();

  EventLoop* loop_;
  bool exiting_;
  Thread thread_;
  MutexLock mutex_;
  Condition cond_;
};

}

#endif


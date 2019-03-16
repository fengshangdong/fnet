#ifndef NET_EVENTLOOPTHREADPOOL_H
#define NET_EVENTLOOPTHREADPOOL_H

#include "Condition.h"
#include "Mutex.h"
#include "Thread.h"
#include "Noncopyable.h"
#include <vector>
#include <functional>

namespace fnet
{

class EventLoop;
class EventLoopThread;

class EventLoopThreadPool : public Noncopyable
{
public:
  EventLoopThreadPool(EventLoop* baseLoop);
  ~EventLoopThreadPool();
  void setThreadNum(int numThreads) { numThreads_ = numThreads; }
  void start();
  EventLoop* getNextLoop();

private:
  EventLoop* baseLoop_;
  bool started_;
  int numThreads_;
  int next_;  // always in loop thread
  std::vector<EventLoopThread*> threads_;
  std::vector<EventLoop*> loops_;
};

}

#endif

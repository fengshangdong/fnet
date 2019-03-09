#ifndef NET_EVENTLOOP_H
#define NET_EVENTLOOP_H

#include "Thread.h"
#include "TimerId.h"
#include "Timestamp.h"
#include "Callbacks.h"
#include "Mutex.h"
#include <memory>
#include <vector>
#include <unistd.h>
#include <sys/eventfd.h>

namespace fnet
{

class Channel;
class Poller;
class TimerQueue;

class EventLoop : public Noncopyable
{
public:
  typedef std::function<void()> Functor;
  EventLoop();
  ~EventLoop();

  void loop();
  void quit();

  void runInLoop(const Functor& cb);
  void queueInLoop(const Functor& cb);

  TimerId runAt(const Timestamp& time, const TimerCallback& cb);
  TimerId runAfter(double delay, const TimerCallback& cb);
  TimerId runEvery(double interval, const TimerCallback& cb);

  void wakeup();

  void updateChannel(Channel* channel);

  void assertInLoopThread()
  {
    if (!isInLoopThread())
    {
      abortNotInLoopThread();
    }
  }

  bool isInLoopThread() const
  {
    return threadId_ == tid();
  }

private:
  void abortNotInLoopThread();
  void handleRead();  // waked up
  void doPendingFunctors();

  typedef std::vector<Channel*> ChannelList;

  bool looping_;
  bool quit_;
  bool callingPendingFunctors_;
  
  int wakeupFd_;

  const pid_t threadId_;
  std::unique_ptr<Poller> poller_;
  std::unique_ptr<TimerQueue> timerQueue_;
  std::unique_ptr<Channel> wakeupChannel_;
  ChannelList activeChannels_;

  MutexLock mutex_;
  std::vector<Functor> pendingFunctors_;
};

}

#endif

#ifndef NET_TIMERQUEUE_H
#define NET_TIMERQUEUE_H

#include "Noncopyable.h"
#include "Timestamp.h"
#include "Mutex.h"
#include "Callbacks.h"
#include "Channel.h"
#include <set>
#include <vector>

namespace fnet
{

class EventLoop;
class Timer;
class TimerId;

class TimerQueue : public Noncopyable
{
public:
  TimerQueue(EventLoop* loop);
  ~TimerQueue();

  TimerId addTimer(const TimerCallback& cb,
                   Timestamp when,
		   double interval);

private:
  typedef std::pair<Timestamp, Timer*> Entry;
  typedef std::set<Entry> TimerList;

  void handleRead();
  std::vector<Entry> getExpired(Timestamp now);
  void reset(const std::vector<Entry>& expired, Timestamp now);

  bool insert(Timer* timer);

  EventLoop* loop_;
  const int timerfd_;
  Channel timerfdChannel_;
  TimerList timers_;
};

}
#endif

#ifndef NET_POLLER_H
#define NET_POLLER_H

#include <map>
#include <vector>

#include "Timestamp.h"
#include "EventLoop.h"
#include "Noncopyable.h"

struct pollfd;

namespace fnet
{

class Channel;

class Poller : public Noncopyable
{
public:
  typedef std::vector<Channel*> ChannelList;

  Poller(EventLoop* loop);
  ~Poller();

  // Must be called in the loop thread.
  Timestamp poll(int timeoutMs, ChannelList* activeChannels);

  // Must be called in the loop thread.
  void updateChannel(Channel* channel);
  void removeChannel(Channel* channel);

  void assertInLoopThread() { ownerLoop_->assertInLoopThread(); }

private:
  void fillActiveChannels(int numEvents,
                          ChannelList* activeChannels) const;

  typedef std::vector<struct pollfd> PollFdList;
  typedef std::map<int, Channel*> ChannelMap;

  EventLoop* ownerLoop_;
  PollFdList pollfds_;
  ChannelMap channels_;
};

}
#endif

#include "EventLoop.h"
#include "Channel.h"
#include "Poller.h"
#include <assert.h>
#include <string.h>
#include <poll.h>
#include <iostream>

using namespace fnet;

__thread EventLoop* t_loopInThisThread = 0;
const int kPollTimeMs = 10000;

EventLoop::EventLoop()
  : looping_(false),
    quit_(false),
    threadId_(tid()),
    poller_(new Poller(this))
{
  if (t_loopInThisThread)
  {
    abortNotInLoopThread();
  }
  else
  {
    t_loopInThisThread = this;
  }
}

EventLoop::~EventLoop()
{
  assert(!looping_);
  t_loopInThisThread = NULL;
}

void EventLoop::loop()
{
  assert(!looping_);
  assertInLoopThread();
  looping_ = true;
  quit_ = false;

  while (!quit_)
  {
    activeChannels_.clear();
    poller_->poll(kPollTimeMs, &activeChannels_);
    for (ChannelList::iterator it = activeChannels_.begin();
	 it != activeChannels_.end(); ++it)
    {
      (*it)->handleEvent();
    }
  }

  std::cout<<"Event Loop stop looping"<<std::endl;
  looping_ = false;
}

void EventLoop::quit()
{
  quit_ = true;
}

void EventLoop::updateChannel(Channel* channel)
{
  assert(channel->ownerLoop() == this);
  assertInLoopThread();
  poller_->updateChannel(channel);
}

void EventLoop::abortNotInLoopThread()
{
  std::cout<<"abortNotInLoopThread" << std::endl;
  exit(0);
}

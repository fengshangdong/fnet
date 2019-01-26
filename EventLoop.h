#ifndef NET_EVENTLOOP_H
#define NET_EVENTLOOP_H

#include "Thread.h"
#include <memory>
#include <vector>

namespace fnet
{

class Channel;
class Poller;

class EventLoop : public Noncopyable
{
 public:
	EventLoop();
	~EventLoop();

	void loop();
	void quit();

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
	typedef std::vector<Channel*> ChannelList;

	bool looping_;
	bool quit_;
	const pid_t threadId_;
	std::unique_ptr<Poller> poller_;
	ChannelList activeChannels_;
};

}

#endif

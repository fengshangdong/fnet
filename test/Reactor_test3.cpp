#include "Channel.h"
#include "EventLoop.h"
#include "Timestamp.h"

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/timerfd.h>

fnet::EventLoop* g_loop;

void timeout(fnet::Timestamp receiveTime)
{
  printf("%s Timeout!\n",receiveTime.toFormattedString().c_str());
  g_loop->quit();
}

int main()
{
  fnet::EventLoop loop;
  g_loop = &loop;

  int timerfd = ::timerfd_create(CLOCK_MONOTONIC, TFD_NONBLOCK | TFD_CLOEXEC);
  fnet::Channel channel(&loop, timerfd);
  channel.setReadCallback(timeout);
  channel.enableReading();

  struct itimerspec howlong;
  bzero(&howlong, sizeof howlong);
  howlong.it_value.tv_sec = 5;
  ::timerfd_settime(timerfd, 0, &howlong, NULL);

  loop.loop();

  ::close(timerfd);
}

#include "EventLoop.h"
#include "Thread.h"

fnet::EventLoop* g_loop;

void threadFunc()
{
  g_loop->loop();
}

int main()
{
  fnet::EventLoop loop;
  g_loop = &loop;
  fnet::Thread t(threadFunc);
  t.start();
  t.join();
}

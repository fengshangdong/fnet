#include "EventLoop.h"
#include "Thread.h"
#include <stdio.h>
#include <unistd.h>

void threadFunc()
{
  printf("threadFunc(): pid = %d, tid = %d\n",
         getpid(), fnet::tid());

  fnet::EventLoop loop;
  loop.loop();
}

int main()
{
  printf("main(): pid = %d, tid = %d\n",
         getpid(), fnet::tid());

  fnet::EventLoop loop;

  fnet::Thread thread(threadFunc);
  thread.start();

  loop.loop();
  pthread_exit(NULL);
}

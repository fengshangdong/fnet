#include "../Timestamp.h"
#include "../ThreadPool.h"
#include "../CountDownLatch.h"

using namespace fnet;

#include <stdio.h>

void print()
{
  printf("tid=%d\n", fnet::tid());
}

void printString(const std::string& str)
{
  printf("tid=%d, str=%s\n", fnet::tid(), str.c_str());
}

int main()
{
  fnet::ThreadPool pool("MainThreadPool");
  pool.start(5);

  pool.taken(print);
  pool.taken(print);

  fnet::Timestamp now(Timestamp::now());
  printf("%s\n", now.toFormattedString().c_str());

  for (int i = 0; i < 1000000; ++i)
  {
    char buf[32];
    snprintf(buf, sizeof buf, "task %d", i);
    pool.taken(std::bind(printString, std::string(buf)));
  }

  fnet::Timestamp now2(Timestamp::now());
  printf("%s\n", now2.toFormattedString().c_str());

  fnet::CountDownLatch latch(1);
  pool.taken(std::bind(&fnet::CountDownLatch::countDown, &latch));
  latch.wait();
  pool.stop();
}


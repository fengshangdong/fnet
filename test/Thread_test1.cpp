#include <pthread.h>
#include <unistd.h>
#include <iostream>

using namespace std;

//面相对象方式创建Thread类
class ThreadBase
{
public:
  ThreadBase()
  {
    cout <<"Thread test ..." <<endl;
  }

  virtual ~ThreadBase()
  {
    cout <<"~Thread test ..." <<endl;
  }

  void Start()
  {
    pthread_create(&threadId_, NULL,ThreadRoutine, this);
  }

  void Join()
  {
    pthread_join(threadId_, NULL);
  }

private:
  static void* ThreadRoutine(void *arg);
  virtual void Run() = 0;
  pthread_t threadId_;
};

void* ThreadBase::ThreadRoutine(void *arg)
{
  ThreadBase* thread = static_cast<ThreadBase*>(arg);
  thread->Run();
  return NULL;
}

/************************************/
/************************************/

class TestThread : public ThreadBase
{
public:
  TestThread(int count) : count_(count)
  {
    cout <<"TestThread ..." <<endl;
  }

  ~TestThread()
  {
    cout <<"~TestThread ..." <<endl;
  }

private:
  void Run()
  {
    while(count_--)
    {
      cout <<"This is a test .." <<endl;
      sleep(1);
    }

  }

  int count_;
};

int main(void)
{
  TestThread t(5);
  t.Start();
  t.Join();

  return 0;
}

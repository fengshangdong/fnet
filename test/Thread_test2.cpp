#include <pthread.h>
#include <unistd.h>
#include <functional>
#include <iostream>

using namespace std;

//基于对象方式创建Thread类
class ThreadBase
{
public:
  typedef std::function<void()> ThreadFunc;

  explicit ThreadBase(const ThreadFunc& func)
    : func_(func)
  {
    cout <<"Thread test ..." <<endl;
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
  void Run() { func_(); }
  static void* ThreadRoutine(void *arg);
  ThreadFunc func_;
  pthread_t threadId_;
};

void* ThreadBase::ThreadRoutine(void *arg)
{
  ThreadBase* thread = static_cast<ThreadBase*>(arg);
  thread->Run();
  return NULL;
}


void ThreadFunc(int count_)
{
    while(count_--)
    {
      cout <<"This is a test .." <<endl;
      sleep(1);
    }
}

int main(void)
{
  ThreadBase t1(std::bind(ThreadFunc, 5));
  t1.Start();
  t1.Join();

  return 0;
}

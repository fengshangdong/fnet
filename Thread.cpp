#include "Thread.h"
#include <unistd.h>
#include <sys/syscall.h>
#include <sys/types.h>
#include <linux/unistd.h>

namespace fnet
{

__thread const char* t_threadName = "main";
__thread pid_t t_cachedTid = 0;

pid_t gettid()
{
  return static_cast<pid_t>(::syscall(SYS_gettid));
}

pid_t tid()
{
  if (t_cachedTid == 0)
  {
    t_cachedTid = gettid();
  }
  return t_cachedTid;
}

const char* name()
{
  return t_threadName;
}

bool isMainThread()
{
  return tid() == ::getpid();
}

Atomic Thread::num_;

void Thread::run()
{
  t_threadName = name_.c_str();
  func_();
  t_threadName = "finished";
}

void* Thread::threadRoutine(void *arg)
{
  Thread* thread = static_cast<Thread*>(arg);
  thread->run();
  return NULL;
}

Thread::Thread(const ThreadFunc& func, const std::string& n)
  : started_(false),
    joined_(false),
    pthreadId_(0),
    tid_(tid()),
    func_(func),
    name_(n)
{
  num_.increment();
}

Thread::~Thread()
{
  if (started_ && !joined_)
  {
    pthread_detach(pthreadId_);
  }
}

void Thread::start()
{
  assert(!started_);
  started_ = true;

  if (pthread_create(&pthreadId_, NULL, threadRoutine, this))
  {
    started_ = false;
    abort();
  }
}

void Thread::join()
{
  assert(started_);
  assert(!joined_);
  joined_ = true;
  pthread_join(pthreadId_, NULL);
}

}

#include "../Mutex.h"
#include <stdio.h>
#include <stdlib.h>

/**************************************
 * exit(1)退出, Global类会调用析构函数, 
 * 但是mutex_成员变量被破坏, 程序卡死
**************************************/
using namespace fnet;

void Exit()
{
  exit(1);
}

class Global
{
public:
  void doit()
  {
    MutexLockGuard lock(mutex_);
    Exit();
  }

  ~Global()
  {
    printf("before ~Global\n");
    MutexLockGuard g(mutex_);
    printf("~Global cleanning\n");
  }

private:
  MutexLock mutex_;
};

Global g_obj;

int main()
{
  g_obj.doit();
}

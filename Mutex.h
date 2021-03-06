#ifndef BASE_MUTEX_H
#define BASE_MUTEX_H

#include "Noncopyable.h"
#include <assert.h>
#include <pthread.h>

/**************************************
 * MutexLock 互斥锁类
 * MutexLock()  : 构造函数,初始化锁
 * ~MutexLock() : 析构函数,销毁锁
 * lock()       : 加锁
 * unlock()     : 解锁
 *
 * MutexLockGuard:
 * RAII手法, 将加锁解锁封装在构造函数和析构函数中
 * 初始化MutexLockGuard对象即可自动加锁自动解锁.
**************************************/
namespace fnet
{

class MutexLock : public Noncopyable
{
public:
  MutexLock()
  {
    pthread_mutex_init(&mutex_, NULL);
  }

  ~MutexLock()
  {
    pthread_mutex_destroy(&mutex_);
  }

  void lock()
  {
    pthread_mutex_lock(&mutex_);
  }

  void unlock()
  {
    pthread_mutex_unlock(&mutex_);
  }

  pthread_mutex_t* getPthreadMutex() /* non-const */
  {
    return &mutex_;
  }

private:
  pthread_mutex_t mutex_;
};

class MutexLockGuard : public Noncopyable
{
public:
  explicit MutexLockGuard(MutexLock& mutex) : mutex_(mutex)
  {
    mutex_.lock();
  }

  ~MutexLockGuard()
  {
    mutex_.unlock();
  }

private:
  MutexLock& mutex_;
};

}

#endif

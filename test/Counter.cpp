#include "../Mutex.h"

using namespace fnet;

class Counter
{
public:
  Counter(): value_(0) {}
  Counter& operator=(const Counter& rhs);
  int value() const;
  int getAndIncrease();
  friend void swap(Counter& a, Counter& b);
private:
  mutable MutexLock mutex_;
  int value_;
};

int Counter::value() const
{
  MutexLockGuard lock(mutex_);
  return value_;
}

int Counter::getAndIncrease()
{
  MutexLockGuard lock(mutex_);
  int ret = value_++;
  return ret;
}
void swap(Counter& a, Counter& b)
{
  MutexLockGuard aLock(a.mutex_);
  MutexLockGuard bLock(b.mutex_);
  int value = a.value_;
  a.value_ = b.value_;
  b.value_ = value;
}

Counter& Counter::operator=(const Counter& rhs)
{
  if(this == &rhs)
    return *this;

  MutexLockGuard myLock(mutex_);
  MutexLockGuard itsLock(rhs.mutex_);
  value_ = rhs.value_;
  return *this;
}

int main()
{
  Counter c;
  c.getAndIncrease();
}

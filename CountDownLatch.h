#ifndef BASE_COUNTDOWNLATCH_H
#define BASE_COUNTDOWNLATCH_H

#include "Mutex.h"
#include "Condition.h"

/**************************************
 * CountDownLatch 倒计时类
 * CountDownLatch() : 初始化计数count
 * wait()           : 加锁等待计数为0
 * countDown()      : 计数减一
**************************************/
namespace fnet
{

class CountDownLatch : public Noncopyable
{
public:
	explicit CountDownLatch(int count)
		: mutex_(),
		condition_(mutex_),
		count_(count)
	{
	}

	void wait()
	{
		MutexLockGuard lock(mutex_);
		while (count_ > 0)
		{
			condition_.wait();
		}
	}

	void countDown()
	{
		MutexLockGuard lock(mutex_);
		--count_;
		if (count_ == 0)
		{
			condition_.notifyAll();
		}
	}

	int getCount() const
	{
		MutexLockGuard lock(mutex_);
		return count_;
	}

private:
	mutable MutexLock mutex_;
	Condition condition_;
	int count_;
};

}
#endif

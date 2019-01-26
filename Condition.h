#ifndef BASE_CONDITION_H
#define BASE_CONDITION_H

#include "Mutex.h"
#include "Noncopyable.h"
#include <pthread.h>
#include <errno.h>

/**************************************
 * Condition 条件变量类
 * Condition()  : 初始化条件变量
 * ~Condition() : 销毁条件变量
 * wait()       : 等待条件变量为真
 * notify()     : 资源可用,使得其中一个条件变量为真
 * notifyAll()  : 状态改变,所有条件变量为真
 *
 * pthread_cond_wait()的实现其实分为四步:
 * (1)解锁:pthread_mutex_unlock(mutex)
 * (2)挂起:条件变量pcond_挂起,等待唤醒
 * (3)唤醒:线程加入就绪队列,不一定立刻执行
 * (4)加锁:pthread_mutex_lock(mutex)
**************************************/
namespace fnet
{

class Condition : public Noncopyable
{
public:
	explicit Condition(MutexLock& mutex): mutex_(mutex)
	{
		pthread_cond_init(&pcond_, NULL);
	}

	~Condition()
	{
		pthread_cond_destroy(&pcond_);
	}

	void wait()
	{
		pthread_cond_wait(&pcond_, mutex_.getPthreadMutex());
	}

	void notify()
	{
		pthread_cond_signal(&pcond_);
	}

	void notifyAll()
	{
		pthread_cond_broadcast(&pcond_);
	}

private:
	MutexLock& mutex_;
	pthread_cond_t pcond_;
};

}
#endif

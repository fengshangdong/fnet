#ifndef BASE_THREAD_H
#define BASE_THREAD_H

#include "Atomic.h"
#include "Noncopyable.h"

#include <pthread.h>
#include <string>
#include <functional>
#include <memory>
#include <assert.h>

/**************************************
 * tid():         调用系统API接口当前取线程真实id
 * name():        返回线程名称
 * isMainThread():判断当前线程是否主线程
 *
 * Thread类:
 * Thread(Func, str):构造函数,构造子线程str,从非成员函数Func处开始执行
 * ~Thread():  析构函数
 * start():    启动线程
 * join():     结束线程
 * started():  判断线程是否启动
 * tid():      启动线程的真实id
 * name():     启动线程的名称
 * num():      启动总线程的个数
 *
 * 1.为什么线程id放在Thread::tid_中, 而tid_是一个shared_ptr?
 * 因为线程id只有在被创建时才能获得,使用shared_ptr是合适的.
 * 2.为什么线程创建的时候没有执行某个类的成员函数,而需要定义ThreadData?
 * 因为pthread_create的线程函数定义为void *func(void *), 无法传递类的成员函数.
**************************************/
namespace fnet
{

class Thread : public Noncopyable
{
public:
	typedef std::function<void ()> ThreadFunc;

	explicit Thread(const ThreadFunc&, const std::string& name = std::string());
	~Thread();

	void start();
	void join();

	bool started() const { return started_; }
	pid_t tid() const { return *tid_; }
	const std::string& name() const { return name_; }

	static int num() { return num_.get(); }

private:
	bool        started_;
	bool        joined_;
	pthread_t   pthreadId_;
	std::shared_ptr<pid_t> tid_;
	ThreadFunc  func_;
	std::string name_;

	static Atomic num_;
};

pid_t tid();
const char* name();
bool isMainThread();

}

#endif

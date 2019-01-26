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

struct ThreadData
{
	typedef Thread::ThreadFunc ThreadFunc;
	ThreadFunc func_;
	std::string name_;
	std::weak_ptr<pid_t> wkTid_;

	ThreadData(const ThreadFunc& func,
			const std::string& name,
			const std::shared_ptr<pid_t>& tid)
		: func_(func),
		name_(name),
		wkTid_(tid)
	{ }

	void runInThread()
	{
		pid_t tid_ = tid();
		std::shared_ptr<pid_t> ptid = wkTid_.lock();

		if (ptid)
		{
			*ptid = tid_;
			ptid.reset();
		}

		t_threadName = name_.c_str();
		func_();
		t_threadName = "finished";
	}
};

void* startThread(void* obj)
{
	ThreadData* data = static_cast<ThreadData*>(obj);
	data->runInThread();
	delete data;
	return NULL;
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

Thread::Thread(const ThreadFunc& func, const std::string& n)
	: started_(false),
	joined_(false),
	pthreadId_(0),
	tid_(new pid_t(0)),
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

	ThreadData* data = new ThreadData(func_, name_, tid_);
	if (pthread_create(&pthreadId_, NULL, &startThread, data))
	{
		started_ = false;
		delete data;
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

#ifndef BASE_THREADPOOL_H
#define BASE_THREADPOOL_H

#include "Noncopyable.h"
#include "Condition.h"
#include "Mutex.h"
#include "Thread.h"

#include <deque>
#include <functional>
#include <vector>

/**************************************
 * ThreadPool 线程池类
 * ThreadPool()  : 构造函数,初始化类.
 * ~ThreadPool() : 析构函数.
 * start()       : 启动线程池, n个线程
 * stop()        : 停止线程池
 * taken()       : 取资源到线程池执行队列
 *
 * vecotr<Thread*> : 线程容器
 * deque<Task>     : 资源执行队列
**************************************/
namespace fnet
{

class ThreadPool : public fnet::Noncopyable
{
public:
	typedef std::function<void ()> Task;

  explicit ThreadPool(const std::string& name = std::string());
  ~ThreadPool();

  void start(int numThreads);
  void stop();

  void taken(const Task& f);

private:
  void runInThread();
  Task take();

  MutexLock mutex_;
  Condition cond_;
  std::string name_;
  std::vector<Thread*> threads_;
  std::deque<Task> queue_;
  bool running_;
};

}

#endif

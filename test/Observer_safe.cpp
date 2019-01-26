#include <algorithm>
#include <vector>
#include <stdio.h>
#include <memory>
#include <functional>
#include "../Mutex.h"

class Observable;

class Observer : public std::enable_shared_from_this<Observer>
{
public:
	virtual ~Observer();
	virtual void update() = 0;
	void observe(Observable* s);
protected:
	Observable* subject_;
};

class Observable
{
public:
	void register_(std::weak_ptr<Observer> x);
	void notifyObservers()
	{
		fnet::MutexLockGuard lock(mutex_);
		Iterator it = observers_.begin();
		while (it != observers_.end())
		{
			std::shared_ptr<Observer> obj(it->lock());
			if (obj)
			{
				obj->update();
				++it;
			}
			else
			{
				printf("notifyObserver() erase\n");
				it = observers_.erase(it);
			}
		}
	}

private:
	mutable fnet::MutexLock mutex_;
	std::vector<std::weak_ptr<Observer> > observers_;
	typedef std::vector<std::weak_ptr<Observer> >::iterator Iterator;
};

Observer:: ~Observer()
{
}

void Observer::observe(Observable* s)
{
	s->register_(shared_from_this());
	subject_ = s;
}

void Observable::register_(std::weak_ptr<Observer> x)
{
	observers_.push_back(x);
}

class Foo : public Observer
{
	virtual void update()
	{
		printf("Foo::update() %p\n", this);
	}
};

int main()
{
	Observable subject;
	{
		/* 作用域之外智能指针p被释放,即new对象Foo也被释放 */
		std::shared_ptr<Foo> p(new Foo);
		p->observe(&subject);
		subject.notifyObservers();
	}
	subject.notifyObservers();
}

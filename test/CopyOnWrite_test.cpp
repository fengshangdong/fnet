#include "../Mutex.h"
#include "../Thread.h"
#include <vector>
#include <memory>
#include <stdio.h>

/**************************************
 * CopyOnWrite 
 * 读取时,使用本地share_ptr指向对象,引用计数为2.
 * 写入时,单独拷贝一份进行读写操作,与原对象无关,实现读写分离.
**************************************/

using namespace fnet;

class Foo
{
public:
	void doit() const;
};

typedef std::vector<Foo> FooList;
typedef std::shared_ptr<FooList> FooListPtr;
FooListPtr g_foos;
MutexLock mutex;

void post(const Foo& f)
{
	printf("post\n");
	MutexLockGuard lock(mutex);
	if(!g_foos.unique())
	{
		g_foos.reset(new FooList(*g_foos));
		printf("copy the whole list\n");
	}
	assert(g_foos.unique());
	g_foos->push_back(f);
}

void traverse()
{
	FooListPtr foos;
	{
		MutexLockGuard lock(mutex);
		foos = g_foos;
		assert(!g_foos.unique());
	}

	for(std::vector<Foo>::const_iterator it = foos->begin();
			it != foos->end(); ++it)
	{
		it->doit();
	}
}

void Foo::doit() const
{
	Foo f;
	post(f);
	printf("doit\n");
}

int main()
{
	{
		g_foos.reset(new FooList);
		Foo f;
		post(f);
		post(f);
		traverse();
	}
	printf("数组大小:%d\n",g_foos->size());
}

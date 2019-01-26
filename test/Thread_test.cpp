#include "../Thread.h"
#include <sys/types.h>
#include <unistd.h>
#include <string>
#include <functional>
#include <stdio.h>

void threadFunc()
{
	printf("tid=%d\n", fnet::tid());
}

void threadFunc2(int x)
{
	printf("tid=%d, x=%d\n", fnet::tid(), x);
}

void threadFunc3()
{
	printf("tid=%d\n", fnet::tid());
	sleep(1);
}

class Foo
{
public:
	explicit Foo(double x)
		:x_(x)
	{
	}

	void memberFunc()
	{
		printf("tid=%d, Foo::x_=%f\n", fnet::tid(), x_);
	}

	void memberFunc2(const std::string& text)
	{
		printf("tid=%d, Foo::x_=%f, text=%s\n", 
				fnet::tid(), x_, text.c_str());
	}

private:
	double x_;
};

int main()
{
	printf("pid=%d, tid=%d\n", ::getpid(), fnet::tid());
	fnet::Thread t1(threadFunc);
	t1.start();
	t1.join();


	fnet::Thread t2(std::bind(threadFunc2, 42),
					"thread for free function with arg");
	t2.start();
	t2.join();


	Foo foo(83.33);
	fnet::Thread t3(std::bind(&Foo::memberFunc, &foo),
					"thread for member function without arg");
	t3.start();
	t3.join();


	fnet::Thread t4(std::bind(&Foo::memberFunc2, std::ref(foo),
					std::string("duo xian chen")));
	t4.start();
	t4.join();
	
	{
		fnet::Thread t5(threadFunc3);
		t5.start();
	}
	sleep(2);
	printf("number of created threads %d\n", fnet::Thread::num());
}

#ifndef BASE_ATOMIC_H
#define BASE_ATOMIC_H

#include "Noncopyable.h"
#include <stdint.h>

/**************************************
 * Atomic 原子计数类
 * get()        : 取值
 * getAndAdd(x) : 取值后增加x
 * increment()  : 自增
 * decrement()  : 自减
**************************************/
namespace fnet
{

class Atomic : public Noncopyable
{
public:
	Atomic()
		: value_(0)
	{
	}

	int get() const
	{
		return __sync_val_compare_and_swap(const_cast<volatile int*>(&value_), 0, 0);
	}

	int getAndAdd(int x)
	{
		return __sync_fetch_and_add(&value_, x);
	}

	void increment()
	{
		getAndAdd(1);
	}

	void decrement()
	{
		getAndAdd(-1);
	}

private:
	volatile int value_;
};

}

#endif

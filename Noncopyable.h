#ifndef BASE_NONCOPYABLE_H
#define BASE_NONCOPYABLE_H

/**************************************
 * 构造函数:     Noncopyable(){}
 * 析构函数:     ~Noncopyable(){}
 * 拷贝构造函数: Noncopyable(const Noncopyable&);
 * 赋值构造函数: const Noncopyable& operator=(const Noncopyable&);
**************************************/
namespace fnet{

class Noncopyable
{
protected:
	Noncopyable(){}
	~Noncopyable(){}
private:
	Noncopyable(const Noncopyable&);
	const Noncopyable& operator=(const Noncopyable&);
};

}

#endif

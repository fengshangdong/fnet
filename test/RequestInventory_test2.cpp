#include "../Mutex.h"
#include "../Thread.h"
#include <set>
#include <memory>
#include <stdio.h>
#include <unistd.h>

class Request;
typedef std::shared_ptr<Request> RequestPtr;

class Inventory
{
public:
	Inventory() : requests_(new RequestList) { }
	void add(const RequestPtr& req)
	{
		fnet::MutexLockGuard lock(mutex_);
		if (!requests_.unique())
		{
			requests_.reset(new RequestList(*requests_));
			printf("Inventory::add() copy on write\n");
		}
		assert(requests_.unique());
		requests_->insert(req);
	}

	void remove(const RequestPtr& req)
	{
		fnet::MutexLockGuard lock(mutex_);
		if (!requests_.unique())
		{
			requests_.reset(new RequestList(*requests_));
			printf("Inventory::remove() copy on write\n");
		}
		assert(requests_.unique());
		requests_->erase(req);
	}

	void printAll() const;

private:
	typedef std::set<RequestPtr> RequestList;
	typedef std::shared_ptr<RequestList> RequestListPtr;

	RequestListPtr getData() const
	{
		fnet::MutexLockGuard lock(mutex_);
		return requests_;
	}

	mutable fnet::MutexLock mutex_;
	RequestListPtr requests_;
};

Inventory g_inventory;

class Request : public std::enable_shared_from_this<Request>
{
public:
	Request() : x_(0) { }
	~Request() 
	{
		x_ = -1;
	}

	void cancel()
	{
		fnet::MutexLockGuard lock(mutex_);
		x_ = 1;
		sleep(1);
		printf("cancel()\n");
		g_inventory.remove(shared_from_this());
	}
	void process()
	{
		fnet::MutexLockGuard lock(mutex_);
		g_inventory.add(shared_from_this());
	}
	void print()
	{
		fnet::MutexLockGuard lock(mutex_);
		printf("print Request\n");
	}

private:
	mutable fnet::MutexLock mutex_;
	int x_;
};

void Inventory::printAll() const
{
	std::set<RequestPtr>::const_iterator it;
	RequestListPtr requests = getData();
	printf("printAll()\n");
	sleep(1);
	for (it = requests->begin();
			 it != requests->end();
			 ++it)
	{
		(*it)->print();
	}
}

void threadFunc()
{
	RequestPtr req(new Request);
	req->process();
	req->cancel();
}

int main()
{
	fnet::Thread thread(threadFunc);
	thread.start();
	usleep(500*1000);
	g_inventory.printAll();
	thread.join();
}

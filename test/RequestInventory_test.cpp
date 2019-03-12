#include "../Mutex.h"
#include "../Thread.h"
#include <set>
#include <memory>
#include <stdio.h>
#include <unistd.h>

using namespace fnet;

class Request;
class Inventory
{
public:
  Inventory() : requests_(new RequestList) { }

  void add(Request* req)
  {
    fnet::MutexLockGuard lock(mutex_);
    if (!requests_.unique())
    {
      requests_.reset(new RequestList(*requests_));
      printf("Inventory::add() -> copy on wriet\n");
    }
    assert(requests_.unique());
    requests_->insert(req);
  }

  void remove(Request* req)
  {
    fnet::MutexLockGuard lock(mutex_);
    if (!requests_.unique())
    {
      requests_.reset(new RequestList(*requests_));
      printf("Inventory::remove() -> copy on write\n");
    }
    assert(requests_.unique());
    requests_->erase(req);
  }

  void printAll() const;

private:
  typedef std::set<Request*> RequestList;
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

class Request
{
public:
  Request() : x_(0) { }
  ~Request() __attribute__ ((noinline))
  {
    fnet::MutexLockGuard lock(mutex_);
    x_ = -1;
    sleep(1);
    g_inventory.remove(this);
  }

  void process()
  {
    fnet::MutexLockGuard lock(mutex_);
    g_inventory.add(this);
  }

  void print() const __attribute__ ((noinline))
  {
    fnet::MutexLockGuard lock(mutex_);
    printf("print Request %p x=%d\n", this, x_);
  }

private:
  mutable fnet::MutexLock mutex_;
  int x_;
};

void Inventory::printAll() const
{
  RequestListPtr requests = getData();
  sleep(1);
  for (std::set<Request*>::const_iterator it = requests->begin();
      it != requests->end(); ++it)
  {
    (*it)->print();
  }
}

void threadFunc()
{
  Request* req = new Request;
  req->process();
  delete req;
}

int main()
{
  Thread thread(threadFunc);
  thread.start();
  usleep(500*1000);
  g_inventory.printAll();
  thread.join();
}

#include <map>
#include <memory>
#include <functional>
#include <assert.h>
#include <stdio.h>
#include "../Mutex.h"

using std::string;
using namespace std::placeholders;

class Stock : public fnet::Noncopyable
{
public:
  Stock(const string& name)
    : name_(name)
  {
    printf(" Stock[%p] %s\n", this, name_.c_str());
  }
  ~Stock()
  {
    printf("~Stock[%p] %s\n", this, name_.c_str());
  }

  const string& key() const { return name_; }

private:
  string name_;
};

namespace version1
{

class StockFactory : public fnet::Noncopyable
{
public:
  std::shared_ptr<Stock> get(const string& key)
  {
    fnet::MutexLockGuard lock(mutex_);
    std::shared_ptr<Stock>& pStock = stocks_[key];
    if (!pStock)
    {
      pStock.reset(new Stock(key));
    }
    return pStock;
  }

private:
  mutable fnet::MutexLock mutex_;
  std::map<string, std::shared_ptr<Stock> > stocks_;
};

}

namespace version2
{

class StockFactory : public fnet::Noncopyable
{
public:
  std::shared_ptr<Stock> get(const string& key)
  {
    std::shared_ptr<Stock> pStock;
    fnet::MutexLockGuard lock(mutex_);
    std::weak_ptr<Stock>& wkStock = stocks_[key];
    pStock = wkStock.lock();
    if (!pStock)
    {
      pStock.reset(new Stock(key));
      wkStock = pStock;
    }
    return pStock;
  }
private:
  mutable fnet::MutexLock mutex_;
  std::map<string, std::weak_ptr<Stock> > stocks_;
};

}

namespace version3
{

class StockFactory : public fnet::Noncopyable
{
public:
  std::shared_ptr<Stock> get(const string& key)
  {
    std::shared_ptr<Stock> pStock;
    fnet::MutexLockGuard lock(mutex_);
    std::weak_ptr<Stock>& wkStock = stocks_[key];
    pStock = wkStock.lock();
    if (!pStock)
    {
      pStock.reset(new Stock(key),
	  std::bind(&StockFactory::deleteStock, this, _1));
      wkStock = pStock;
    }
    return pStock;
  }

private:
  void deleteStock(Stock* stock)
  {
    if (stock)
    {
      printf("deleteStock[%p]\n", stock);
      fnet::MutexLockGuard lock(mutex_);
      stocks_.erase(stock->key());
    }
    delete stock;
  }

  mutable fnet::MutexLock mutex_;
  std::map<string, std::weak_ptr<Stock> > stocks_;
};

}

namespace version4
{

class StockFactory : public std::enable_shared_from_this<StockFactory>,
                     public fnet::Noncopyable
{
public:
  std::shared_ptr<Stock> get(const string& key)
  {
    std::shared_ptr<Stock> pStock;
    fnet::MutexLockGuard lock(mutex_);
    std::weak_ptr<Stock>& wkStock = stocks_[key];
    pStock = wkStock.lock();
    if (!pStock) {
      pStock.reset(new Stock(key), 
	  std::bind(&StockFactory::deleteStock,
	    shared_from_this(),
	    _1));
      wkStock = pStock;
    }
    return pStock;
  }

private:
  void deleteStock(Stock* stock)
  {
    if (stock)
    {
      printf("deleteStock[%p]\n", stock);
      fnet::MutexLockGuard lock(mutex_);
      stocks_.erase(stock->key());
    }
    delete stock;
  }

  mutable fnet::MutexLock mutex_;
  std::map<string, std::weak_ptr<Stock> > stocks_;
};

}

class StockFactory : public std::enable_shared_from_this<StockFactory>,
                     public fnet::Noncopyable
{
public:
  std::shared_ptr<Stock> get(const string& key)
  {
    std::shared_ptr<Stock> pStock;
    fnet::MutexLockGuard lock(mutex_);
    std::weak_ptr<Stock>& wkStock = stocks_[key];
    pStock = wkStock.lock();
    if (!pStock) {
      pStock.reset(new Stock(key),
	  std::bind(&StockFactory::weakDeleteCallback,
	    std::weak_ptr<StockFactory>(shared_from_this()),
	    _1));
      wkStock = pStock;
    }
    return pStock;
  }

private:
  static void weakDeleteCallback(const std::weak_ptr<StockFactory>& wkFactory,
      Stock* stock)
  {
    std::shared_ptr<StockFactory> factory(wkFactory.lock());
    if (factory) {
      factory->removeStock(stock);
    }

    delete stock;
  }

  void removeStock(Stock* stock)
  {
    if (stock)
    {
      printf("deleteStock[%p]\n", stock);
      fnet::MutexLockGuard lock(mutex_);
      stocks_.erase(stock->key());
    }
  }

  mutable fnet::MutexLock mutex_;
  std::map<string, std::weak_ptr<Stock> > stocks_;
};

void testLongLifeFactory()
{
  std::shared_ptr<StockFactory> factory(new StockFactory);
  {
    std::shared_ptr<Stock> stock = factory->get("IBM");
    std::shared_ptr<Stock> stock2 = factory->get("IBM");
  }
}

void testShortLifeFactory()
{
  std::shared_ptr<Stock> stock;
  {
    std::shared_ptr<StockFactory> factory(new StockFactory);
    stock = factory->get("IBM");
    std::shared_ptr<Stock> stock2 = factory->get("IBM");
  }
}

int main()
{
#if 0
  version1::StockFactory sf1;
  version2::StockFactory sf2;
  version3::StockFactory sf3;
  std::shared_ptr<version3::StockFactory> sf4(new version3::StockFactory);
  std::shared_ptr<StockFactory> sf5(new StockFactory);

  {
    std::shared_ptr<Stock> s1 = sf1.get("stock1");
  }

  {
    std::shared_ptr<Stock> s2 = sf2.get("stock2");
  }

  {
    std::shared_ptr<Stock> s3 = sf3.get("stock3");
  }

  {
    std::shared_ptr<Stock> s4 = sf4->get("stock4");
  }

  {
    std::shared_ptr<Stock> s5 = sf5->get("stock5");
  }

#endif
  testLongLifeFactory();
  testShortLifeFactory();
}


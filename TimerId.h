#ifndef NET_TIMERID_H
#define NET_TIMERID_H

namespace fnet
{

class Timer;

class TimerId
{
public:
  explicit TimerId(Timer* timer)
    : value_(timer)
  {
  }

private:
  Timer* value_;
};

}

#endif

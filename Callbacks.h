#ifndef BASE_CALLBACKS_H
#define BASE_CALLBACKS_H

#include <memory>
#include <functional>

namespace fnet
{
  class TcpConnection;
  typedef std::shared_ptr<TcpConnection> TcpConnectionPtr;

  typedef std::function<void()> TimerCallback;
  typedef std::function<void (const TcpConnectionPtr&)> ConnectionCallback;
  typedef std::function<void (const TcpConnectionPtr&,
                              const char* data,
                              ssize_t len)> MessageCallback;

}

#endif 

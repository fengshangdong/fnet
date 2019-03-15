#ifndef BASE_CALLBACKS_H
#define BASE_CALLBACKS_H

#include "Timestamp.h"
#include <memory>
#include <functional>

namespace fnet
{
  class Buffer;
  class TcpConnection;
  
  typedef std::shared_ptr<TcpConnection> TcpConnectionPtr;

  typedef std::function<void()> TimerCallback;
  typedef std::function<void (const TcpConnectionPtr&)> ConnectionCallback;
  typedef std::function<void (const TcpConnectionPtr&,
                              Buffer* buf,
                              Timestamp)> MessageCallback;
  typedef std::function<void (const TcpConnectionPtr&)> CloseCallback;

}

#endif 

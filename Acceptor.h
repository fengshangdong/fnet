#ifndef NET_ACCEPTOR_H
#define NET_ACCEPTOR_H

#include "Noncopyable.h"
#include "Channel.h"
#include "Socket.h"

namespace fnet
{

class EventLoop;
class InetAddress;

class Acceptor : public Noncopyable
{
public:
  typedef std::function<void (int sockfd,
                              const InetAddress&)> NewConnectionCallback;

  Acceptor(EventLoop* loop, const InetAddress& listenAddr);

  void setNewConnectionCallback(const NewConnectionCallback& cb)
  { newConnectionCallback_ = cb; }

  bool listenning() const { return listenning_; }
  void listen();

private:
  void handleRead();

  EventLoop* loop_;
  Socket acceptSocket_;
  Channel acceptChannel_;
  NewConnectionCallback newConnectionCallback_;
  bool listenning_;
};

}

#endif

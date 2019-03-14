#ifndef NET_TCPSERVER_H
#define NET_TCPSERVER_H

#include "Callbacks.h"
#include "TcpConnection.h"
#include "Noncopyable.h"
#include <map>

namespace fnet
{

class Acceptor;
class EventLoop;

class TcpServer : public Noncopyable
{
public:
  TcpServer(EventLoop* loop, const InetAddress& listenAddr);
  ~TcpServer();

  void start();

  void setConnectionCallback(const ConnectionCallback& cb)
  { connectionCallback_ = cb; }

  void setMessageCallback(const MessageCallback& cb)
  { messageCallback_ = cb; }

private:
  typedef std::map<std::string, TcpConnectionPtr> ConnectionMap;

  void newConnection(int sockfd, const InetAddress& peerAddr);

  EventLoop* loop_;  // the acceptor loop
  const std::string name_;
  std::unique_ptr<Acceptor> acceptor_; // avoid revealing Acceptor
  ConnectionCallback connectionCallback_;
  MessageCallback messageCallback_;
  bool started_;
  int nextConnId_;  // always in loop thread
  ConnectionMap connections_;
};

}

#endif

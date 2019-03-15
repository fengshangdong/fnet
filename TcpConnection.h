#ifndef NET_TCPCONNECTION_H
#define NET_TCPCONNECTION_H

#include "Buffer.h"
#include "Callbacks.h"
#include "InetAddress.h"
#include "Timestamp.h"
#include "Noncopyable.h"
#include <memory>
#include <functional>

namespace fnet
{

class Channel;
class EventLoop;
class Socket;

///
/// TCP connection, for both client and server usage.
///
class TcpConnection : public Noncopyable,
                      public std::enable_shared_from_this<TcpConnection>
{
public:
  /// Constructs a TcpConnection with a connected sockfd
  TcpConnection(EventLoop* loop,
                const std::string& name,
                int sockfd,
                const InetAddress& localAddr,
                const InetAddress& peerAddr);
  ~TcpConnection();

  EventLoop* getLoop() const { return loop_; }
  const std::string& name() const { return name_; }
  const InetAddress& localAddress() { return localAddr_; }
  const InetAddress& peerAddress() { return peerAddr_; }
  bool connected() const { return state_ == kConnected; }

  void setConnectionCallback(const ConnectionCallback& cb)
  { connectionCallback_ = cb; }

  void setMessageCallback(const MessageCallback& cb)
  { messageCallback_ = cb; }

  void setCloseCallback(const CloseCallback& cb)
  { closeCallback_ = cb; }

  void connectEstablished();   // should be called only once
  void connectDestroyed();

private:
  enum StateE { kConnecting, kConnected, kDisconnected, };

  void setState(StateE s) { state_ = s; }
  void handleRead(Timestamp receiveTime);
  void handleWrite();
  void handleClose();
  void handleError();

  EventLoop* loop_;
  std::string name_;
  StateE state_;
  // we don't expose those classes to client.
  std::unique_ptr<Socket> socket_;
  std::unique_ptr<Channel> channel_;
  InetAddress localAddr_;
  InetAddress peerAddr_;
  ConnectionCallback connectionCallback_;
  MessageCallback messageCallback_;
  CloseCallback closeCallback_;
  Buffer inputBuffer_;
};

typedef std::shared_ptr<TcpConnection> TcpConnectionPtr;

}

#endif

#ifndef NET_SOCKET_H
#define NET_SOCKET_H

#include "Noncopyable.h"

namespace fnet
{

class InetAddress;

class Socket : public Noncopyable
{
public:
  explicit Socket(int sockfd)
    : sockfd_(sockfd)
  { }

  ~Socket();

  int fd() const { return sockfd_; }

  void bindAddress(const InetAddress& localaddr);
  void listen();

  int accept(InetAddress* peeraddr);

  void setReuseAddr(bool on);

private:
  const int sockfd_;
};

}
#endif

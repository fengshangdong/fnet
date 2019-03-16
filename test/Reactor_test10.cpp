#include "TcpServer.h"
#include "EventLoop.h"
#include "InetAddress.h"
#include <stdio.h>

std::string message1;
std::string message2;

void onConnection(const fnet::TcpConnectionPtr& conn)
{
  if (conn->connected())
  {
    printf("onConnection(): tid=%d new connection [%s] from %s\n",
           fnet::tid(),
           conn->name().c_str(),
           conn->peerAddress().toHostPort().c_str());
    if (!message1.empty())
      conn->send(message1);
    if (!message2.empty())
      conn->send(message2);
    conn->shutdown();
  }
  else
  {
    printf("onConnection(): tid=%d connection [%s] is down\n",
           fnet::tid(),
           conn->name().c_str());
  }
}

void onMessage(const fnet::TcpConnectionPtr& conn,
               fnet::Buffer* buf,
               fnet::Timestamp receiveTime)
{
  printf("onMessage(): tid=%d received %zd bytes from connection [%s] at %s\n",
         fnet::tid(),
         buf->readableBytes(),
         conn->name().c_str(),
         receiveTime.toFormattedString().c_str());

  buf->retrieveAll();
}

int main(int argc, char* argv[])
{
  printf("main(): pid = %d\n", getpid());

  int len1 = 100;
  int len2 = 200;

  if (argc > 2)
  {
    len1 = atoi(argv[1]);
    len2 = atoi(argv[2]);
  }

  message1.resize(len1);
  message2.resize(len2);
  std::fill(message1.begin(), message1.end(), 'A');
  std::fill(message2.begin(), message2.end(), 'B');

  fnet::InetAddress listenAddr(9981);
  fnet::EventLoop loop;

  fnet::TcpServer server(&loop, listenAddr);
  server.setConnectionCallback(onConnection);
  server.setMessageCallback(onMessage);
  if (argc > 3)
  {
    server.setThreadNum(atoi(argv[3]));
  }

  server.start();
  loop.loop();
}

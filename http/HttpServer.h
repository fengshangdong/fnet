#ifndef NET_HTTP_HTTPSERVER_H
#define NET_HTTP_HTTPSERVER_H

#include "TcpServer.h"
#include "HttpContext.h"

namespace fnet
{

class HttpContext;
class HttpRequest;
class HttpResponse;

class HttpServer : public Noncopyable
{
public:
  typedef std::function<void (const HttpRequest&,
                              HttpResponse*)> HttpCallback;

  HttpServer(EventLoop* loop, const InetAddress& listenAddr);

  void setHttpCallback(const HttpCallback& cb)
  { httpCallback_ = cb; }

  void setThreadNum(int numThreads)
  { server_.setThreadNum(numThreads); }

  void start()
  { server_.start(); }

private:
  void onConnection(const TcpConnectionPtr& conn);
  void onMessage(const TcpConnectionPtr& conn,
                 Buffer* buf,
                 Timestamp receiveTime);
  void onRequest(const TcpConnectionPtr&, const HttpRequest&);
  
  void setContext(const HttpContext& context)
  { context_ = context; }

  HttpContext& getContext()
  { return context_; }

  HttpContext context_;
  TcpServer server_;
  HttpCallback httpCallback_;
};

}

#endif

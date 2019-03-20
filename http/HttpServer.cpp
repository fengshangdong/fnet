#include "HttpServer.h"
#include "HttpContext.h"
#include "HttpRequest.h"
#include "HttpResponse.h"
#include <functional>
#include <iostream>
#include <string>

using namespace fnet;
using namespace std::placeholders;

void defaultHttpCallback(const HttpRequest&, HttpResponse* resp)
{
  resp->setStatusCode(HttpResponse::k404NotFound);
  resp->setStatusMessage("Not Found");
  resp->setCloseConnection(true);
}

HttpServer::HttpServer(EventLoop* loop,
                       const InetAddress& listenAddr)
  : server_(loop, listenAddr),
    httpCallback_(defaultHttpCallback)
{
  server_.setConnectionCallback(
      std::bind(&HttpServer::onConnection, this, _1));
  server_.setMessageCallback(
      std::bind(&HttpServer::onMessage, this, _1, _2, _3));
}

void HttpServer::onConnection(const TcpConnectionPtr& conn)
{
  if (conn->connected())
  {
    setContext(HttpContext());
  }
}

void HttpServer::onMessage(const TcpConnectionPtr& conn,
                           Buffer* buf,
                           Timestamp receiveTime)
{
  HttpContext& context = getContext();

  if (!context.parseRequest(buf, receiveTime))
  {
    conn->send("HTTP/1.1 400 Bad Request\r\n\r\n");
    conn->shutdown();
  }

  if (context.gotAll())
  {
    onRequest(conn, context.request());
    context.reset();
  }
}

void HttpServer::onRequest(const TcpConnectionPtr& conn, const HttpRequest& req)
{
  const std::string& connection = req.getHeader("Connection");
  bool close = connection == "close" ||
    (req.getVersion() == HttpRequest::kHttp10 && connection != "Keep-Alive");
  HttpResponse response(close);
  httpCallback_(req, &response);
  Buffer buf;
  response.appendToBuffer(&buf);
  conn->send(buf.retrieveAsString());
  if (response.closeConnection())
  {
    conn->shutdown();
  }
}


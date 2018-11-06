#ifndef SOCKET_HPP
#define SOCKET_HPP

#ifdef _WIN32
#include <WinSock2.h>
#include <WS2tcpip.h>
#pragma comment(lib, "Ws2_32.lib")
#else
#include <unistd.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#endif

#ifndef SOCKET
#define SOCKET int
#endif

#ifndef INVALID_SOCKET
#define INVALID_SOCKET -1
#endif

#ifndef SOCKET_ERROR
#define SOCKET_ERROR -1
#endif

// plan 0
class SocketImpl
{
public:
  SocketImpl(const int& family, const int& type, const int& protocol);
private:
  SOCKET m_sockfd;
};

class Socket
{
public:
  Socket();
  virtual ~Socket();
protected:
  void Bind();
  void Listen();
  void Accept();
  void Connect();
  
  void Send();
  void Recv();

  void SendTo();
  void RecvFrom();

  void Close();
private:
  SocketImpl m_impl;
};

// plan 1
#if 0
#ifdef _WIN32
class SocketImpl
{
private:
  SOCKET m_fd;
};
#else
class SocketImpl
{

};
#endif
#endif
class TcpSocket : public Socket
{
public:

};
// plan 2
#ifdef _WIN32
class TcpSocketImpl : public TcpSocket
{
public:

};
#else
class TcpSocketImpl : public TcpSocket
{
public:

};
#endif

class UdpSocket : public Socket
{
public:

};

#ifdef _WIN32
class UdpSocketImpl : public UdpSocket
{
public:

};
#else
class UdpSocketImpl : public UdpSocket
{
public:

};
#endif

class TcpServer : public TcpSocket
{
public:

};

class TcpClient : public TcpSocket
{
public:

};

#endif
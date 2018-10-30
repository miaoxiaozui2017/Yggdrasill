#ifndef SOCKET_HPP
#define SOCKET_HPP

#include <iostream>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <netdb.h>
#include <arpa/inet.h>

// TODO: rewrite socket class
// server use server sockfd
// client use client sockfd
class Socket
{
public:
  Socket() : m_sockType(NONE), m_sockFd(-1), m_clientFd(-1){}
  
  bool Init(const int & servicePort);
  bool Init(const int & servicePort, const char* ipAddr);

  bool Connect();
  bool IsConnected();
  bool DisConnect();
protected:
  enum Ty
  {
    NONE,
    SERVER,
    CLIENT
  };
  Ty m_sockType;
  int m_sockFd;
  int m_clientFd;
private:
  struct sockaddr_in m_sockAddr;
  unsigned int m_sinSize;
  bool m_isConnected;
};
#endif
#ifndef TCPCLIENT_HPP
#define TCPCLIENT_HPP

#include <vector>
#include <string.h>
#include <sys/types.h>
#include <iostream>
#include <fcntl.h>
#include <errno.h>

#ifdef _WIN32
#include <WinSock2.h>
#include <WS2tcpip.h>
#pragma comment(lib, "Ws2_32.lib")

#define CRS_SOCKET SOCKET
#define CRS_INVALID_SOCKET INVALID_SOCKET
#define CRS_SOCKET_ERROR SOCKET_ERROR
#elif __linux__
#include <unistd.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>

#define CRS_SOCKET int
#define CRS_INVALID_SOCKET -1
#define CRS_SOCKET_ERROR -1
#endif

#define BUFFER_SIZE 4096

class TcpClient
{
public:
  TcpClient() : m_transform(nullptr), m_sockFd(CRS_INVALID_SOCKET), m_isConnected(false) {}
  ~TcpClient();
  bool Init();
  bool Connect(const char * addr, const unsigned short & servicePort);
  bool DisConnect();
  bool IsConnected() { return m_isConnected; }

  bool Recv(std::string& msg);
  bool Send(const std::string& msg);
  char * m_transform;
  int m_transLength;

private:
  CRS_SOCKET m_sockFd;
  struct sockaddr_in m_servAddr;
  bool m_isConnected;
  unsigned short m_bufferLength;
};

#endif
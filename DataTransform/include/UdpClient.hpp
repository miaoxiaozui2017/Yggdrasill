#ifndef UDPCLIENT_HPP
#define UDPCLIENT_HPP

#include <iostream>
#include <string.h>
#include <sys/types.h>

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

class UdpClient
{
public:
  UdpClient() : m_sockFd(CRS_INVALID_SOCKET) {}
  ~UdpClient() { Release(); }
  bool Init();
  bool Release();
  bool SendTo(const char * data, const size_t & length, const char * serverIp, const int & port);
  bool RecvFrom(char * data, size_t & length, char ** ppaddr, unsigned short * pport);

private:
  CRS_SOCKET m_sockFd;
  struct sockaddr_in m_servAddr;
  struct sockaddr_in m_sendAddr;
  char m_buffer[512];
  size_t m_bufferLength;
};

#endif
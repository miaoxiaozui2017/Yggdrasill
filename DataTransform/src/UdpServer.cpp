#include "UdpServer.hpp"

bool UdpServer::Init()
{
#ifdef _WIN32
  WSADATA wsaData;
  WORD sockVersion = MAKEWORD(2, 2);
  if (WSAStartup(sockVersion, &wsaData) != 0) return false;
  m_sockFd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
#elif __linux__
  m_sockFd = socket(AF_INET, SOCK_DGRAM, 0);
#endif
  if (m_sockFd == CRS_INVALID_SOCKET)
  {
    std::cout << "Create socket error." << std::endl;
    return false;
  }
#ifdef _WIN32
  int timeout = 1;
#elif __linux__
  struct timeval timeout = { 0, 1000 };
#endif
  setsockopt(m_sockFd, SOL_SOCKET, SO_RCVTIMEO, (char*)&timeout, sizeof(timeout));
  return true;
}

bool UdpServer::Bind(const unsigned short & servicePort)
{
#ifdef _WIN32
  m_servAddr.sin_addr.S_un.S_addr = INADDR_ANY;
#elif __linux__
  bzero(&m_servAddr, sizeof(m_servAddr));
  m_servAddr.sin_addr.s_addr = htonl(INADDR_ANY); //inet_addr("127.0.0.1");
#endif
  m_servAddr.sin_family = AF_INET;
  m_servAddr.sin_port = htons(servicePort);
  if (bind(m_sockFd, (struct sockaddr*)&m_servAddr, sizeof(m_servAddr)) != CRS_SOCKET_ERROR)
    return true;
  else return false;
}

bool UdpServer::Release()
{
  if (m_sockFd != CRS_INVALID_SOCKET)
  {
#ifdef _WIN32
    closesocket(m_sockFd);
    WSACleanup();
#elif __linux__
    close(m_sockFd);
#endif
    m_sockFd = CRS_INVALID_SOCKET;
  }
  return true;
}

bool UdpServer::SendTo(const char * data, const size_t & length, const char * clientIp, const int & port)
{
  m_sendAddr.sin_family = AF_INET;
  m_sendAddr.sin_port = htons(port);
#ifdef _WIN32
  inet_pton(AF_INET, clientIp, &m_sendAddr);
#elif __linux__
  m_sendAddr.sin_addr.s_addr = inet_addr(clientIp);
  bzero(&(m_sendAddr.sin_zero), 8);
#endif

  int ret = sendto(m_sockFd, data, length, 0, (struct sockaddr*)&m_sendAddr, sizeof(m_sendAddr));
  if (ret == 0)
  {
    std::cout << "Connection closed." << std::endl;
    return false;
  }
  if (ret < 0)
  {
    std::cout << "Error." << std::endl;
    return false;
  }
  if (ret != static_cast<int>(length))
  {
    std::cout << "Send length error." << std::endl;
    return false;
  }
  return true;
}

bool UdpServer::RecvFrom(char * data, size_t & length, char ** ppaddr, unsigned short * pport)
{
  int addr_len = sizeof(m_servAddr);
  length = recvfrom(m_sockFd, data, length, 0/*MSG_DONTWAIT*/, (struct sockaddr*)&m_servAddr,
#ifdef _WIN32
    /*(socklen_t*)*/&addr_len
#elif __linux__
    (socklen_t*)&addr_len
#endif
  );
  if (length == SIZE_MAX)
  {
    // std::cout << "UDP timeout." << std::endl;
    return false;
  }

  if (ppaddr)
  {
    // TODO : add support to get addr
// #ifdef _WIN32
//     inet_ntop(AF_INET, &m_sendAddr, *ppaddr, 0);
// #elif __linux__
//     *ppaddr = inet_ntoa(m_sendAddr.sin_addr);
// #endif
  }

  if (pport)
    *pport = ntohs(m_sendAddr.sin_port);
  return true;
}

bool UdpServer::RecvFrom(std::string & msg)
{
  m_bufferLength = 512;
  if (!RecvFrom(m_buffer, m_bufferLength, NULL, NULL))
    return false;
  msg.assign(m_buffer, m_buffer + m_bufferLength);
  return true;
}
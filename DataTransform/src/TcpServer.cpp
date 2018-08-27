#include "TcpServer.hpp"

bool Socket::Init(const int & servicePort)
{
  m_sockType = SERVER;
  m_sockFd = socket(AF_INET, SOCK_STREAM, 0);
  if (m_sockFd == -1)
    return false;
  struct sockaddr_in my_addr;
  my_addr.sin_family = AF_INET;
  my_addr.sin_port = htons(servicePort);
  my_addr.sin_addr.s_addr = INADDR_ANY;
  bzero(&(my_addr.sin_zero), 8);
  if ((bind(m_sockFd, (struct sockaddr *)&my_addr, sizeof(struct sockaddr))) == -1)
    return false;
  if(listen(m_sockFd, 10) == -1)
    return false;
  m_sinSize = sizeof(struct sockaddr_in);
  return true;
}

bool Socket::Init(const int & servicePort, const char* ipAddr)
{
  m_sockType = CLIENT;
  m_sockFd = socket(AF_INET, SOCK_STREAM, 0);
  if (m_sockFd == -1)
    return false;
  m_sockAddr.sin_family=AF_INET;
  m_sockAddr.sin_port=htons(servicePort);
  m_sockAddr.sin_addr.s_addr = inet_addr(ipAddr);
  bzero(&(m_sockAddr.sin_zero),8);
  return true;
}

bool Socket::Connect()
{
  switch(m_sockType)
  {
  case NONE:
    std::cerr << "This socket instance was not init." << std::endl;
    return false;
  case SERVER:
    m_clientFd = accept(m_sockFd, (struct sockaddr *)&m_sockAddr, &m_sinSize);
    return (m_clientFd != -1);
  case CLIENT:
    if (connect(m_sockFd, (struct sockaddr*)&m_sockAddr, sizeof(struct sockaddr)) != -1)
    {
      m_isConnected = true;
      return true;
    }
    return false;
  default:
    return false;
  }
}

bool Socket::IsConnected()
{
  switch(m_sockType)
  {
  case NONE:
    std::cerr << "This socket instance was not init." << std::endl;
    return false;
  case SERVER:
    return (m_clientFd != -1);
  case CLIENT:
    return m_isConnected;
  default:
    return false;
  }
}

bool Socket::DisConnect()
{
  switch(m_sockType)
  {
  case NONE:
    std::cerr << "This socket instance was not init." << std::endl;
    return false;
  case SERVER:
    if (m_clientFd != -1)
    {
      close(m_clientFd);
      return true;
    }
    return false;
  case CLIENT:
    if (m_isConnected)
    {
      close(m_sockFd);
      m_isConnected = false;
      return true;
    }
    return false;
  default:
    return false;
  }
}
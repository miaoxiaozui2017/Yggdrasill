#include "TcpClient.hpp"

TCPClient::~TCPClient()
{
  DisConnect();
  if (m_transform != nullptr)
  {
    delete[] m_transform;
    m_transform = nullptr;
  }
}
bool TCPClient::Init()
{
#ifdef _WIN32
  WSADATA wsaData;
  WORD sockVersion = MAKEWORD(2, 2);
  if (WSAStartup(sockVersion, &wsaData) != 0) return false;
  m_sockFd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
#elif __linux__
  m_sockFd = socket(AF_INET, SOCK_STREAM, 0);
#endif
  if (m_sockFd == CRS_INVALID_SOCKET)
  {
    return false;
  }
  int ret;
#ifdef _WIN32
  unsigned long ul = 1;
  ret = ioctlsocket(m_sockFd, FIONBIO, (unsigned long *)&ul);
#elif __linux__
  int flags = fcntl(m_sockFd, F_GETFL, 0);
  ret = fcntl(m_sockFd, F_SETFL, flags | O_NONBLOCK);
  // int imode = 1;
  // ioctl(sock_fd, FIONBIO, &imode);
#endif
  if (ret == CRS_SOCKET_ERROR)
  {
    std::cout << "Socket unblocked error." << std::endl;
    return false;
  }
  return true;
}

bool TCPClient::Connect(const char * addr, const unsigned short & servicePort)
{
  m_servAddr.sin_family=AF_INET;
  m_servAddr.sin_port=htons(servicePort);
#ifdef _WIN32
  m_servAddr.sin_addr.S_un.S_addr = inet_addr(addr);
#elif __linux__
  m_servAddr.sin_addr.s_addr = inet_addr(addr);
  bzero(&(m_servAddr.sin_zero),8);
#endif
  if (connect(m_sockFd, (struct sockaddr*)&m_servAddr, sizeof(struct sockaddr)) != CRS_SOCKET_ERROR)
  {
    m_isConnected = true;
    // create transform buffer
    if (m_transform == nullptr)
    {
      m_transform = new char[BUFFER_SIZE];
      m_transLength = BUFFER_SIZE;
    }
    if (m_transform != nullptr)
      return true;
  }
  return false;
}

bool TCPClient::DisConnect()
{
  if (m_isConnected)
  {
#ifdef _WIN32
    closesocket(m_sockFd);
    WSACleanup();
#elif __linux__
    close(m_sockFd);
#endif
    m_sockFd = CRS_INVALID_SOCKET;
    m_isConnected = false;
    return true;
  }
  return false;
}

bool TCPClient::Recv(std::string& msg)
{
  if (m_transform == nullptr)
  {
    std::cout << "[Recv]Client is not working." << std::endl;
    return false;
  }
  m_transLength = BUFFER_SIZE;
  m_transLength = recv(m_sockFd, m_transform, m_transLength, 0);
  
  if (m_transLength == CRS_SOCKET_ERROR)
  {
#ifdef _WIN32
  int err = WSAGetLastError();
  if (err == WSAEWOULDBLOCK)
  {
    std::cout << "Recv WOULDBLOCK error." << std::endl;
  }
  else if (err == WSAETIMEDOUT)
  {
    std::cout << "Recv TIMEOUT error." << std::endl;
  }
  else if (err == WSAENETDOWN)
  {
    std::cout << "Recv NETDOWN error." << std::endl;
  }
#elif __linux__
  if (errno == EWOULDBLOCK)
  {
    // std::cout << "Recv WOULDBLOCK error." << std::endl;
    return false;
  }
#endif
  }
  
  if (m_transLength == BUFFER_SIZE)
  {
    std::cout << "[Recv]Package is too large." << std::endl;
    return false;
  }
  m_transform[m_transLength] = '\0';
  msg = m_transform;
  return true;
}

// transform command
bool TCPClient::Send(const std::string& msg)
{
  int sentSize = send(m_sockFd, msg.c_str(), msg.length(), 0);
  if (sentSize == static_cast<int>(msg.length()))
  {
    return true;
  }
  std::cout << "We can't send it by one package." << std::endl;
  return false;
}


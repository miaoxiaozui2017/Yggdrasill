#include "Socket.hpp"

SocketImpl::SocketImpl(const int& family, const int& type, const int& protocol)
{
  if ( (m_sockfd = socket(family, type, protocol)) < 0)
    
}
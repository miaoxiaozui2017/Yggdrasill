#ifndef SERIAL_HPP
#define SERIAL_HPP

#include "SerialLinuxImpl.hpp"
#include "SerialWindowsImpl.hpp"

class Serial
{
public:
  Serial() = default;
  bool Open(const SerialConfig& conf) { return m_impl.Open(conf); }
  bool Close() { return m_impl.Close(); }
  template<typename T>
  bool Send(const std::vector<T>& buffer) { return m_impl.Send(buffer); }

  template<typename T>
  bool Recv(std::vector<T>& buffer) { return m_impl.Recv(buffer); }
  
  ~Serial() = default;
private:
  SerialImpl m_impl;
};

#endif
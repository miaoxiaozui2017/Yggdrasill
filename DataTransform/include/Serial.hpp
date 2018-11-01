#ifndef SERIAL_HPP
#define SERIAL_HPP
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <termios.h>
#include <errno.h>

#include <string>
#include <vector>

struct SerialConfig
{
  std::string serialName;
  int speed;
  int databits;
  char parity;
  int stopbits;
  bool isBlocked;
};

class Serial
{
public:
  Serial()
    : m_fd(-1)
  {}
  bool Open(const SerialConfig& conf);
  bool Close();
  template<typename T>
  bool Send(const std::vector<T>& buffer)
  {
    if (m_fd == -1)
    {
      fprintf(stderr, "Port is not Opened.\n");
      return false;
    }
    int byte = 0;
    if ( (byte = write(m_fd, &buffer[0], buffer.size())) == -1)
    {
      perror("Error in sending.");
      return false;
    }
    return true;
  }

  template<typename T>
  bool Recv(std::vector<T>& buffer)
  {
    if (m_fd == -1)
    {
      fprintf(stderr, "Port is not Opened.\n");
      return false;
    }
    int byte = 0;
    char buf[1024];
    if ( (byte = read(m_fd, &buf[0], 1024)) == -1)
    {
      //perror("Error in recv.");
      return false;
    }
    buffer.assign(buf, buf + byte);
    fprintf(stdout, "%d bytes recv.\n", byte);
    return true;
  }
  
  ~Serial() { Close(); }
private:
  bool Config(const SerialConfig& conf);
  speed_t GetBaudRate(const int& speed);
  int m_fd;
};

template<>
bool Serial::Send(const std::vector<int>& buffer)
{
  fprintf(stderr, "We cannot send integer yet.\n");
  return false;
  // TODO : add support for integer vector if need.
}
#endif
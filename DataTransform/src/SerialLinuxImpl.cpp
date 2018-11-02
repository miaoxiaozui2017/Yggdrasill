#include "SerialLinuxImpl.hpp"

#ifndef _WIN32

bool SerialImpl::Open(const SerialConfig& conf)
{
  // Open serial port
  if ( (m_fd = open(conf.serialName.c_str(), O_RDWR | O_NOCTTY | O_NDELAY)) == -1)
  {
    perror("Serial can not open");
    return false;
  }
  return Config(conf);
}

bool SerialImpl::Close()
{
  if (m_fd != -1)
  {
    if (close(m_fd) != 0)
    {
      perror("Close failed");
      return false;
    }
    m_fd = -1;
  }
  return true;
}

bool SerialImpl::Config(const SerialConfig& conf)
{
  // Config serial port
  struct termios options;
  // Get the current options for the port
  tcgetattr(m_fd, &options);

  // Set the baud rates
  speed_t speed = B0;
  if ( (speed = GetBaudRate(conf.speed)) == B0)
  {
    fprintf(stderr, "Undefined baud rate.\n");
    return false;
  }
  cfsetispeed(&options, speed);
  cfsetospeed(&options, speed);
  // Enable the receiver and set local mode
  options.c_cflag |= (CLOCAL | CREAD);
  
  // Mask the charactor size bits
  options.c_cflag &= ~CSIZE;
  // Select databits
  switch (conf.databits)
  {
    case 7:
      options.c_cflag |= CS7;
      break;
    case 8:
      options.c_cflag |= CS8;
      break;
    default:
      fprintf(stderr, "Unsupported databits.\n");
      return false;
  }

  // Select parity
  switch (conf.parity)
  {
    case 'n':
    case 'N':
      options.c_cflag &= ~PARENB;// Clear parity enable
      options.c_iflag &= ~INPCK; // Enable parity checking
      break;
    case 'o':
    case 'O':
      options.c_cflag |= (PARODD | PARENB); // 设置为奇效验
      options.c_iflag |= INPCK;             // Disnable parity checking
      break;
    case 'e':
    case 'E':
      options.c_cflag |= PARENB;     // Enable parity
      options.c_cflag &= ~PARODD;   // 转换为偶效验
      options.c_iflag |= INPCK;       // Disnable parity checking
      break;
    case 'S':
    case 's':  //as no parity
      options.c_cflag &= ~PARENB;
      options.c_cflag &= ~CSTOPB;
      break;
    default:
      fprintf(stderr, "Unsupported parity.\n");
      return false;
  }

  // Set stopbits
  switch (conf.stopbits)
  {
    case 1:
      options.c_cflag &= ~CSTOPB;
      break;
    case 2:
      options.c_cflag |= CSTOPB;
      break;
    default:
      fprintf(stderr,"Unsupported stopbits.\n");
      return false;
  }
  if (conf.parity != 'n' && conf.parity != 'N')
    options.c_iflag |= INPCK;
  tcflush(m_fd, TCIFLUSH);
  options.c_cc[VTIME] = 150; // over time 15 seconds
  options.c_cc[VMIN] = 0; // Update the options and do it NOW
  if (tcsetattr(m_fd, TCSANOW, &options) != 0)
  {
    perror("tcsetattr error.");
    return false;
  }

  // set isblocked
  if (conf.isBlocked)
  {
    fcntl(m_fd, F_SETFL, 0);
  }
  // else
  // {
  //   fcntl(m_fd, F_SETFL, FNDELAY);
  // }
  return true;
}

speed_t SerialImpl::GetBaudRate(const int& speed)
{
  switch (speed)
  {
    case 2400: return B2400;
    case 4800: return B4800;
    case 9600: return B9600;
    case 19200: return B19200;
    case 38400: return B38400;
    case 57600: return B57600;
    case 115200: return B115200;
    case 230400: return B230400;
    case 460800: return B460800;
    case 500000: return B500000;
    default: return B0;
  }
}
#endif
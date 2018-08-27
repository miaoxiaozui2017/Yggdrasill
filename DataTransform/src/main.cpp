#include <iostream>
#include "UdpClient.hpp"

int main(int argc, char ** argv)
{
  UdpClient udpClient;
  udpClient.Init();
  char buffer[512];
  size_t length = sizeof(buffer);
  char serverIp[] = "127.0.0.1";
  int port = 10000;
  char * data = &buffer[0];
  char * pServerIp = &serverIp[0];
  while (1)
  {
    memcpy(buffer, "Request", 8);
    if (udpClient.SendTo(data, length, pServerIp, port))
    {
      std::cout << "Sent" << std::endl;
    }
    sleep(1);
  }
}
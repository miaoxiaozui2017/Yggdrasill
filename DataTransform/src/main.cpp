#include <iostream>
#include <fstream>
#include <string>
#include "UdpClient.hpp"

#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <malloc.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/ioctl.h>
#include <sys/types.h>  
#include <netinet/in.h>  
#include <arpa/inet.h>  
#include <linux/if.h>

#include <tclap/CmdLine.h>
#include <json/json.h>

#include "Serial.hpp"

#define BUF_SIZE 1024
using namespace std;

using namespace TCLAP;
using namespace Json;

static int getIP(string net_name, string &strIP)
{
    int sock_fd;
    struct ifconf conf;
    struct ifreq *ifr;
    char buff[BUF_SIZE] = {0};
    int num;
    int i;

    sock_fd = socket(PF_INET, SOCK_DGRAM, 0);
    if ( sock_fd < 0 )     
		return -1;

    conf.ifc_len = BUF_SIZE;
    conf.ifc_buf = buff;

    if ( ioctl(sock_fd, SIOCGIFCONF, &conf) < 0 )
    {
        close(sock_fd);
        return -1;
    }

    num = conf.ifc_len / sizeof(struct ifreq);
    ifr = conf.ifc_req;

    for(i = 0; i < num; i++)
    {
        struct sockaddr_in *sin = (struct sockaddr_in *)(&ifr->ifr_addr);

        if ( ioctl(sock_fd, SIOCGIFFLAGS, ifr) < 0 )
        {
                close(sock_fd);
                return -1;
        }

        if ( (ifr->ifr_flags & IFF_UP) && strcmp(net_name.c_str(),ifr->ifr_name) == 0 )
        {
                strIP = inet_ntoa(sin->sin_addr);
                close(sock_fd);

                return 0;
        }
		
        ifr++;
    }
	
    close(sock_fd);

    return -1;
}

int main(int argc, char ** argv)
{
  // string ip;
  // getIP("enp1s0f0", ip);
  // cout << ip << endl;
  // return 0;

  // UdpClient udpClient;
  // udpClient.Init();
  // char buffer[512];
  // size_t length = sizeof(buffer);
  // char serverIp[] = "127.0.0.1";
  // int port = 10000;
  // char * data = &buffer[0];
  // char * pServerIp = &serverIp[0];
  // while (1)
  // {
  //   memcpy(buffer, "Request", 8);
  //   if (udpClient.SendTo(data, length, pServerIp, port))
  //   {
  //     std::cout << "Sent" << std::endl;
  //   }
  //   sleep(1);
  // }
  try
  {
    CmdLine cmd("What is this XXX message", ' ', "0.1");

    ValueArg<std::string> nameArg("c", "name", "Path of config file", true, "homer", "FileName");
    cmd.add(nameArg);

    cmd.parse(argc, argv);

    std::string name = nameArg.getValue();
    std::cout << name << std::endl;
    // 读取json文件
    Value root;
    std::ifstream fin(name);
    if (!fin.is_open())
    {
      std::cerr << "Open failed." << std::endl;
      return 1;
    }
    fin >> root;
    SerialConfig conf;
    conf.serialName = root["serialName"].asString();
    conf.speed;
    conf.databits = root["databits"].asInt();
    conf.parity = root["parity"].asString().at(0);
    conf.stopbits = root["stopbits"].asInt();
    conf.isBlocked = root["isBlocked"].asBool();


    // std::string encoding = root.get("encoding", "UTF-8").asString();
    // std::cout << encoding << std::endl;
    // std::string key1 = root["key1"].asString();
    // std::cout << key1 << std::endl;
    // const Value key2 = root["key2"];

    // for (size_t i = 0; i < key2.size(); i++)
    // {
    // 	std::cout << key2[static_cast<int>(i)].asString() << std::endl;
    // }

    std::cout << root << std::endl;
    fin.close();
  }
  catch (ArgException &e)
  {
    std::cerr << "error : " << e.error() << " for arg " << e.argId() << std::endl;
  }
}


#include <string.h>
#include <time.h>

#include <vector>
// #include "utils.hpp"
// #include "DataBuffer.hpp"
// #include "ImuSolver.hpp"



//serial port set function
// void setTermios(struct termios *pNewtio, unsigned short uBaudRate)
// {
//     bzero(pNewtio,sizeof(struct termios));
//     pNewtio->c_cflag = uBaudRate|CS8|CREAD|CLOCAL;
//     //pNewtio->c_iflag = IGNPAR;
//     pNewtio->c_iflag = 0;
//     pNewtio->c_oflag = 0;
//     pNewtio->c_lflag = 0;
    // pNewtio->c_cc[VINTR] = 0;
    // pNewtio->c_cc[VQUIT] = 0;
    // pNewtio->c_cc[VERASE] = 0;
    // pNewtio->c_cc[VKILL] = 0;
    // pNewtio->c_cc[VEOF] = 4;
    // pNewtio->c_cc[VTIME] = 5;
    // pNewtio->c_cc[VMIN] = 0;
    // pNewtio->c_cc[VSWTC] = 0;
    // pNewtio->c_cc[VSTART] = 0;
    // pNewtio->c_cc[VSTOP] = 0;
    // pNewtio->c_cc[VSUSP] = 0;
    // pNewtio->c_cc[VEOL] = 0;
    // pNewtio->c_cc[VREPRINT] = 0;
    // pNewtio->c_cc[VDISCARD] = 0;
    // pNewtio->c_cc[VWERASE] = 0;
    // pNewtio->c_cc[VLNEXT] = 0;
    // pNewtio->c_cc[VEOL2] = 0;
//}
/*
int main(int argc,char **argv)
{
  clock_t start, end;
  int fd;
  int nCount,nTotal;
  int i,j,m;
  int readByte = 0;
  struct termios oldtio,newtio;
  char dev[] = "/dev/ttyUSB0";
  char buffer[512];
  // if((argc!=3)||(sscanf(argv[1],"%d",&nTotal)!=1))
  // {
  //     printf("Usage:./serial count data!\n");
  //     return -1;
  // }

  if((fd=open(dev,O_RDWR|O_NOCTTY|O_NDELAY))<0) //O_NOCTTY防止程序受键盘控制中止操作键等影响 O_NDELAY不关心端口另一端是否已经连接
  {
    printf("Can't open serial port!\n");    
    return -1;
  }

  tcgetattr(fd,&oldtio);
  setTermios(&newtio,B115200);
  //tcflush(fd,TCIFLUSH);   //清空输入输出
  tcsetattr(fd,TCSANOW,&newtio);

  // Send data
  // for(i=0;i<nTotal;i++)
  // {
  //     nCount = write(fd,argv[2],strlen(argv[2]));
  //     printf("send data OK!count=%d\n",nCount);
  //     sleep(1);
  // }
  //receive data
  unsigned short test = 0;
  //ConvertToWord(0x01, 0x01, test);
  // ImuSolver imuSolver;
  // std::vector<unsigned char> splits;
  // splits.push_back('*');
  // splits.push_back('\r');
  // splits.push_back('\n');
  // DataBuffer<unsigned char> dataBuffer(splits);
  std::vector<std::vector<unsigned char> > segments;
  start = clock();
  while (1)
  {
    readByte = read(fd, buffer, 512);
    //std::cout << readByte;
    // printf("readlength=%d\n",ReadByte);
    // Buffer[ReadByte]='\0';
    // SplitString(Buffer, ",")
    if (readByte > 0)
    {
      // std::cout << readByte;
      //dataBuffer.Charge((unsigned char *)buffer, readByte);
      //dataBuffer.Discharge(segments);
      //std::cout << segments.size() << std::endl;
      for (std::vector<std::vector<unsigned char> >::iterator it = segments.begin(); it != segments.end(); it++)
      {
        std::string seg(it->begin(), it->end());
        //if (!imuSolver.Solve(*it))
        {
          end = clock();
          //std::cout << "time = " << (double)(end - start) / CLOCKS_PER_SEC << std::endl;
          start = end;
          continue;
        }
        //imuSolver.m_imuPackage.Output();
        // for (std::vector<unsigned char>::iterator i = it->begin(); i != it->end(); i++)
        // {
        //   printf("0x%02x ", *i);
        // }
        // std::cout << std::endl;
        // std::cout << seg << std::endl;
        //std::cout << it->size() << std::endl;
      }
    }
    
      // printf("%s",buffer);
      //usleep(50000);
    // else printf("Read data failure times=%d\n",j);
  }     
  printf("Receive data finished!\n");
  tcsetattr(fd,TCSANOW,&oldtio);
  close(fd);
  return 0;

}
*/
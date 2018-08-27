#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <termios.h>
#include <errno.h>
#include <string.h>
#include <time.h>

#include <vector>
#include "utils.hpp"
#include "DataBuffer.hpp"
#include "ImuSolver.hpp"

#include <fstream>

//serial port set function
void setTermios(struct termios *pNewtio, unsigned short uBaudRate)
{
    bzero(pNewtio,sizeof(struct termios));
    pNewtio->c_cflag = uBaudRate|CS8|CREAD|CLOCAL;
    //pNewtio->c_iflag = IGNPAR;
    pNewtio->c_iflag = 0;
    pNewtio->c_oflag = 0;
    pNewtio->c_lflag = 0;
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
}

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
  ImuSolver imuSolver;
  std::vector<unsigned char> splits;
  splits.push_back('*');
  splits.push_back('\r');
  splits.push_back('\n');
  DataBuffer<unsigned char> dataBuffer(splits);
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
      dataBuffer.Charge((unsigned char *)buffer, readByte);
      dataBuffer.Discharge(segments);
      //std::cout << segments.size() << std::endl;
      for (std::vector<std::vector<unsigned char> >::iterator it = segments.begin(); it != segments.end(); it++)
      {
        std::string seg(it->begin(), it->end());
        if (!imuSolver.Solve(*it))
        {
          end = clock();
          std::cout << "time = " << (double)(end - start) / CLOCKS_PER_SEC << std::endl;
          start = end;
          continue;
        }
        imuSolver.m_imuPackage.Output();
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

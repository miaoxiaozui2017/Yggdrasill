#include <iostream>
#include <unistd.h>

int main(int, char **)
{
  static const char *szfilename = "/tmp/my_test_daemon.txt";
  FILE *pFile = fopen(szfilename, "a+");
  while (1)
  {
    fprintf(pFile, "hello world\n");
		fflush(pFile);
		sleep(2);
  }
}

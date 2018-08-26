#include <iostream>
#include <unistd.h>

int main(int, char **)
{
  static const char *szfilename = "/tmp/my_test_daemon.txt";
  FILE *pFile = fopen(szfilename, "a+");
  while (1)
  {
    pid_t pid = getpid();
    pid_t ppid = getppid();
    fprintf(pFile, "pid = %d, ppid = %d, hello world\n", pid, ppid);
		fflush(pFile);
		sleep(2);
  }
  exit(0);
}

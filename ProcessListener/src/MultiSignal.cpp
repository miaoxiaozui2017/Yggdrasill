#include <cstdio>
#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>

int count = 0;
void sigint_handler(int signo, siginfo_t *info, void *myact)
{
  printf("%d Get SIGINT.\n", count);
  count++;
  if (count > 3)
  {
    exit(0);
  }
}

int main(int argc, char ** argv)
{
  // fork()
  // pid_t pid = fork();
  
  // switch (pid)
  // {
  //   case -1:
  //     perror("fork");
  //     break;
  //   case 0:
  //     printf("In sub process.\n");
  //     printf("pid = %d, ppid = %d\n", getpid(), getppid());
  //     exit(0);
  //   default:
  //     printf("main process.\n");
  //     printf("spid = %d, pid = %d\n", pid, getpid());
  //     break;
  // }
  // signal 子进程继承
  struct sigaction action1;
  sigemptyset(&action1.sa_mask);
  action1.sa_flags = SA_SIGINFO;
  action1.sa_sigaction = sigint_handler;

  sigaction(SIGINT, &action1, NULL);
  sigset_t set;
  sigemptyset(&set);

  char buffer[50];
  getcwd(&buffer[0], 50);
  strcat(buffer, "/Yggdrasill");
  printf("%s\n", buffer);
  // execv(buffer, argv);
  pid_t pid = fork();
  switch (pid)
  {
    case -1:
      perror("fork");
      break;
    case 0:
      printf("In sub process.\n");
      printf("pid = %d, ppid = %d\n", getpid(), getppid());
      printf("Sleep\n");
      
      sleep(5);
      exit(0);
    default:
      printf("main process.\n");
      printf("spid = %d, pid = %d\n", pid, getpid());
      wait(NULL);
      printf("waited.\n");
      break;
  }
  return 0;
  // 父进程控制子进程
  // 单个父进程控制多个子进程
  // 处理exec signal重置问题
}
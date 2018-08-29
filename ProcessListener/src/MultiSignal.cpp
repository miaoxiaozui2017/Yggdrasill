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

void sigchd_handler(int signo, siginfo_t *info, void *myact)
{
  printf("%d Get SIGINT.\n", count);
  count++;
  if (count > 3)
  {
    exit(0);
  }
}

void sigset_display(sigset_t *pset, const char *setname)
{
  size_t len = sizeof(pset->__val) / sizeof(pset->__val[0]);
  for (size_t i = 0; i < len; i++)
  {
    //printf("set[%lu]:%016lx ", i, set.__val[i]);
    printf("%s[%lu]:%016lx\n", setname, i, pset->__val[i]);
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

  //fork() execv()
  struct sigaction action1;
  sigemptyset(&action1.sa_mask);
  action1.sa_flags = SA_SIGINFO;
  action1.sa_sigaction = sigint_handler;

  sigaction(SIGINT, &action1, NULL);

  pid_t child;


  char buffer[50];
  getcwd(&buffer[0], 50);
  strcat(buffer, "/Yggdrasill");
  printf("%s\n", buffer);

  // 子进程继承父进程屏蔽
  sigset_t set;
  sigset_t oldset;
  sigemptyset(&set);
  sigemptyset(&oldset);
  sigaddset(&set, SIGHUP);
  sigprocmask(SIG_BLOCK, &set, &oldset);
  sigset_display(&set, "M");

  pid_t pid = fork();
  switch (pid)
  {
    case -1:
      perror("fork");
      break;
    case 0:
      printf("In sub process.\n");
      printf("pid = %d, ppid = %d\n", getpid(), getppid());
      sigemptyset(&set);
      sigprocmask(SIG_SETMASK, &set, &oldset);
      sigset_display(&oldset, "SO");
      // 子进程确实会继承父进程的屏蔽
      printf("Sleep\n");
      execv(buffer, argv);
      sleep(10);
      exit(0);
    default:
      printf("main process.\n");
      printf("spid = %d, pid = %d\n", pid, getpid());

      // 父进程控制子进程
      //sleep(2);
      printf("main kill\n");
      // 处理exec signal重置问题
      //kill(pid, SIGHUP); 
      child = wait(NULL);
      printf("waited %d.\n", child);
      sleep(10);
      break;
  }

  return 0;
  
  // 单个父进程监控多个子进程

}
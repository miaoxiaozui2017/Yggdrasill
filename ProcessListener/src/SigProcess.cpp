#include <cstdio>
#include <iostream>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

using namespace std;

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
  cout << "Signal management." << endl;
  //signal set
  sigset_t set;
  //cout << sizeof(set.__val) / sizeof(set.__val[0]) << endl;
  //sigset_t oldset;
  sigemptyset(&set);
  //sigemptyset(&oldset);
  //sigaddset(&set, SIGHUP);
  sigaddset(&set, SIGQUIT);
  sigaddset(&set, SIGINT);
  //sigfillset(&set);

  // signal block
  //sigprocmask(SIG_SETMASK/*SIG_BLOCK SIG_UNBLOCK SIG_SETMASK*/, &set, &oldset);
  
  // signal pending
  // sigpending(&set);
  // for (size_t i = 0; i < sizeof(set.__val) / sizeof(set.__val[0]); i++)
  // {
  //   printf("set[%lu]:%016lx ", i, set.__val[i]);
  //   printf("oldset[%lu]:%016lx\n", i, oldset.__val[i]);
  // }
  // cout << endl;

  // signal install
  struct sigaction action1, action2;
  sigemptyset(&action1.sa_mask);
  action1.sa_flags = SA_SIGINFO;
  action1.sa_sigaction = sigint_handler;

  sigaction(SIGINT, &action1, NULL);
  // Recvieved SIGINT signal will call sigint_handler
  // sigaction usage

  perror("perror info");
  // stderr [perror info: success]
  
  printf("unblocked\n");
  sleep(10);
  sigprocmask(SIG_BLOCK, &set, NULL);
  // SIGINT blocked
  printf("blocked\n");
  if (sigismember(&set, SIGINT))
  {
    printf("SIGINT in set.\n");
  }
  sleep(5);
  // send SIGINT signal but blocked
  sigset_t pendset;
  sigset_t suspendset;
  sigemptyset(&suspendset);
  sigemptyset(&pendset);
  sigpending(&pendset);
  // check signal in blocked
  if (sigismember(&pendset, SIGINT))
  {
    printf("Pending SIGINT.");
  }
  // get the signal
  
  int i = 0;
  while (i < 10)
  {
    printf("Suspend.\n");
    // wait last signal
    sigsuspend(&suspendset);
    for (size_t i = 0; i < sizeof(set.__val) / sizeof(set.__val[0]); i++)
  {
    //printf("set[%lu]:%016lx ", i, set.__val[i]);
    printf("suspendset[%lu]:%016lx\n", i, suspendset.__val[i]);
  }
  cout << endl;
  }
  sigprocmask(SIG_UNBLOCK, &set, NULL);
  printf("unblocked\n");
  while (1)
  {
    sleep(1);
  }
  

  for (size_t i = 0; i < sizeof(set.__val) / sizeof(set.__val[0]); i++)
  {
    //printf("set[%lu]:%016lx ", i, set.__val[i]);
    printf("pendset[%lu]:%016lx\n", i, pendset.__val[i]);
  }
  cout << endl;
  return 0;
}
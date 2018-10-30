#include <iostream>
#include <fstream>
#include <string>
#include <unistd.h>
// getpid() / getppid() / daemon() / get_current_dir_name() / fork()
#include <sys/wait.h>
// struct sigaction
// #include <sys/types.h>
#include <sys/stat.h>
// umask()
#include <fcntl.h>
// open()
#include <string.h>
// memset()

#include <tclap/CmdLine.h>
#include <json/json.h>

#include "../../DataTransform/include/UdpServer.hpp"

using namespace TCLAP;
using namespace Json;
// input
// 日志存放目录
// 节点名称
// ip地址？
// 开放端口号
struct Msg
{
  enum MsgT
  {
    ACK,
    QUIT
  };
  MsgT msgType;
};

// 信号全局变量
static sig_atomic_t reap;
static sig_atomic_t terminate;

static pid_t master_pid;
static pid_t worker_pid;
static const char *szfilename = "/tmp/my_test_daemon.txt";

void signal_handler(int signo);
void worker(long i);
void init_signals();
void start_worker_processes();
bool start_daemon();

// 信号处理
struct signal_t
{
  int signo;
  void (*handler)(int signo);
};

// 指定信号处理函数
signal_t signals[] = {
    {SIGHUP, signal_handler},
    {SIGQUIT, signal_handler},
    {SIGTERM, signal_handler},
    {SIGALRM, signal_handler},
    {SIGINT, signal_handler},
    {SIGIO, signal_handler},
    {SIGCHLD, signal_handler},
    {SIGSYS, SIG_IGN},
    {SIGPIPE, SIG_IGN},
    {0, NULL}};

// 信号处理函数
void signal_handler(int signo)
{
  signal_t *sig;
  for (sig = signals; sig->signo != 0; sig++)
  {
    if (sig->signo == signo)
      break;
  }
  switch (signo)
  {
  case SIGTERM:
  case SIGINT:
  {
    terminate = 1;
    break;
  }
  case SIGCHLD:
  {
    reap = 1;
    break;
  }
  default:
  {
    break;
  }
  }
}

bool start_daemon()
{
  int fd;

  switch (fork())
  {
  case -1:
    printf("fork() failed\n");
    return false;

  case 0:
    break;

  default:
    exit(0);
  }

  if (setsid() == -1)
  {
    printf("setsid() failed\n");
    return false;
  }

  switch (fork())
  {
  case -1:
    printf("fork() failed\n");
    return false;

  case 0:
    break;

  default:
    exit(0);
  }

  umask(0);
  chdir("/");

  long maxfd;
  if ((maxfd = sysconf(_SC_OPEN_MAX)) != -1)
  {
    for (fd = 0; fd < maxfd; fd++)
    {
      close(fd);
    }
  }

  fd = open("/dev/null", O_RDWR);
  if (fd == -1)
  {
    printf("open(\"/dev/null\") failed\n");
    return false;
  }

  if (dup2(fd, STDIN_FILENO) == -1)
  {
    printf("dup2(STDIN) failed\n");
    return false;
  }

  if (dup2(fd, STDOUT_FILENO) == -1)
  {
    printf("dup2(STDOUT) failed\n");
    return false;
  }

  if (dup2(fd, STDERR_FILENO) == -1)
  {
    printf("dup2(STDERR) failed\n");
    return false;
  }

  if (fd > STDERR_FILENO)
  {
    if (close(fd) == -1)
    {
      printf("close() failed\n");
      return false;
    }
  }

  return true;
}

void worker(long i)
{
  sigset_t set;
  int childpid;
  int res;
  //子进程继承父进程对信号的屏蔽，所以这里需要解除对信号的屏蔽
  sigemptyset(&set);
  if (sigprocmask(SIG_SETMASK, &set, NULL) == -1)
  {
    printf("work sigprocmask failed\n");
    return;
  }

  pid_t pid = fork();
  switch (pid)
  {
  case -1:
    printf("exec fork failure\n");
    return;
  case 0:
    res = execl("/home/jlurobot/GitLabSync/Yggdrasill/build/Yggdrasill", NULL);
    if (res < 0)
    {
      perror("error on exec\n");
      exit(0);
    }
    break;
  default:
    wait(&childpid);
    break;
  }
  
 
  exit(0);
  // FILE *pFile = fopen(szfilename, "a+");

  // while (true)
  // {
  // 	if (terminate)
  // 	{
  // 		exit(0);
  // 	}

  // 	fprintf(pFile, "pid = %ld hello world\n", i);
  // 	fflush(pFile);
  // 	sleep(2);
  // }

  // if (NULL != pFile)
  // {
  // 	fclose(pFile);
  // 	pFile = NULL;
  // }
}

void init_signals()
{
  signal_t *sig;
  struct sigaction sa;

  for (sig = signals; sig->signo != 0; sig++)
  {
    memset(&sa, 0, sizeof(struct sigaction));
    sa.sa_handler = sig->handler;
    sigemptyset(&sa.sa_mask);

    if (sigaction(sig->signo, &sa, NULL) == -1)
    {
      printf("sigaction error\n");
      return;
    }
  }
}

void start_worker_processes()
{
  pid_t pid;
  pid = fork();
  switch (pid)
  {
  case -1:
    printf("exec fork failure\n");
    return;
  case 0:
    worker((long)getpid());
    break;
  default:
    worker_pid = pid;
    break;
  }
}

int main(int argc, char **argv)
{
	// 程序工作流程
  // 数据定义
  // 消息队列、进程管理表、节点列表
  // 同时处于工作状态的模块
  // 消息主循环、进程管理器、网络收发

	// 由systemd启动程序
	// 读取本地配置文件config.json，设置节点名称
	// 如果失败则退出，并记录日志
	// 调用daemon进入守护进程
	// 读取网络配置，获取节点ip地址
  // 发送广播报文，接收其他节点返回的请求，与网络中的其他节点建立连接
  // 初始化接收消息队列，将消息队列与网络链接绑定
  // 进入消息队列处理循环
  // 获取消息
  // 处理消息
  // 消息内容1：执行程序
  // fork子进程，将该子进程加入进程管理器
  // 进程管理器监视子进程退出，并回收资源
  // (optional)监视子进程、强制删除
  // 子进程执行程序代码
  // 消息内容2：广播消息
  // 将对应节点名称和IP加入到本地节点列表中
  // 回传节点自身信息
  // 消息内容3：转发消息
  // 将应用层消息转发到远端
  // 消息内容4：离线请求
  // 删除对应节点
  // 消息内容5：扫描请求
  // 返回可用程序
  // 消息内容6：注册请求
  // 注册程序
  // 消息内容7：部署请求
  // 详见Deployer

  // 广播报文作为消息处理
  // 单独线程维护节点列表，定期发送确认链接报文更新在线状况
  
  // 程序报错异常退出时
  // 广播发送离线请求
  // 停止所有子进程
  // --------------------
	// tclap
	// 一般用途
	// 判断输入参数
	// 1、参数用途不对、或者没有参数
	// 输出帮助信息
	// 2、正确的参数用法
	// 引导执行对应功能
	try
	{
		// Parse
		CmdLine cmd("What is this XXX message", ' ', "0.1");

		ValueArg<std::string> nameArg("n", "name", "Path of config file", true, "homer", "FileName");
		cmd.add(nameArg);

		cmd.parse(argc, argv);

		// Open file
		std::ofstream fout;
		fout.open(szfilename, std::ios_base::app);
		if (!fout.is_open())
		{
			std::cerr << "Cannot open " << szfilename << std::endl;
			return 1;
		}
		//FILE *pFile = fopen(szfilename, "a+");
		pid_t pid = getpid();
		pid_t ppid = getppid();
		//fprintf(pFile, "pid = %d, ppid = %d, hello world\n", pid, ppid);
		fout << "pid = " << pid << ", ppid = " << ppid << ", hello world.\n";
		std::string name = nameArg.getValue();
		//std::cout << name << std::endl;
		//fprintf(pFile, "name = %s\n", name.c_str());
		fout << "name = " << name << std::endl;
		// 读取json文件
		Value root;
		std::ifstream fin(name);
		if (!fin.is_open())
		{
			std::cerr << "Open failed." << std::endl;
			return 1;
		}
		fin >> root;
		// std::string encoding = root.get("encoding", "UTF-8").asString();
		// std::cout << encoding << std::endl;
		std::string nodeName = root["NodeName"].asString();
		//std::cout << key1 << std::endl;
		//fprintf(pFile, "key1 = %s\n", key1.c_str());
		fout << "NodeName = " << nodeName << std::endl;
		// const Value key2 = root["key2"];
		// for (size_t i = 0; i < key2.size(); i++)
		// {
		// 	std::cout << key2[static_cast<int>(i)].asString() << std::endl;
		// }
		// std::cout << root << std::endl;
		fout << root << std::endl;
		fin.close();
		sleep(2);
		fout .close();
		return 0;

		// 调用和监控子进程
		sigset_t set;

		//初始化各信号，给个信号安装处理函数
		init_signals();

		//主进程以daemon方式运行
		start_daemon();

		//设置信号掩码，暂时屏蔽信号，后续通过sigsuspend主动获取信号
		sigemptyset(&set);
		sigaddset(&set, SIGHUP);
		sigaddset(&set, SIGQUIT);
		sigaddset(&set, SIGTERM);
		sigaddset(&set, SIGALRM);
		sigaddset(&set, SIGINT);
		sigaddset(&set, SIGIO);
		sigaddset(&set, SIGCHLD);

		if (sigprocmask(SIG_BLOCK, &set, NULL) == -1)
		{
			printf("sigprocmask failed\n");
			return 0;
		}

		sigemptyset(&set);

		//启动worker进程
		start_worker_processes();

		//主进程循环监控
		while (true)
		{
			//sigsuspend(const sigset_t *mask))用于在接收到某个信号之前, 临时用mask替换进程的信号掩码, 并暂停进程执行，直到收到信号为止。
			//sigpending(&set);
			if (terminate)
			{
				kill(worker_pid, SIGTERM);		//通知子进程关闭
				waitpid(worker_pid, NULL, 0); //等待回收资源，避免僵尸进程

				//主进程退出时删除测试文件
				if (0 == access(szfilename, F_OK))
				{
					remove(szfilename);
				}

				exit(0);
			}

			if (reap)
			{
				waitpid(worker_pid, NULL, 0); //若子进程退出，需等待回收资源，避免僵尸进程
				start_worker_processes();			//拉起新的子进程
			}

			sleep(1);
		}
	}
	catch (ArgException &e)
	{
		std::cerr << "error : " << e.error() << " for arg " << e.argId() << std::endl;
	}
	return 0;
	// daemon process
	if (daemon(0, 0) != 0)
	{
		std::cerr << "daemon = -1" << std::endl;
		return 1;
	}

	std::cerr << "daemon = 0" << std::endl;
	std::cout << "sub pid : " << getpid() << std::endl;
	std::cout << "sub ppid : " << getppid() << std::endl;
	std::cout << "sub pwd : " << get_current_dir_name() << std::endl;
	while (1)
		;
	return 0;
}
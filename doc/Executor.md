## Executor

任务：执行应用层程序进程。
# 原理说明
Executor接收来自ProcessLister的执行消息后，加载执行其他程序。一种方式是调用控制台命令实现，但该进程的子进程中间会包含一个bash进程，无法直接管理进程。  
我们直接采用fork()创建子进程，exec()加载程序。需要返回进程号。
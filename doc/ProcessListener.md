## ProcessListener

# 原理说明
操作系统启动时执行，负责管理该节点的进程资源，数据链路健康情况。
## 1. P2P建立链接、维持链接、断开连接方法
  每个节点包含自己的IP地址信息、MAC地址信息、节点名称信息  
  执行开始后广播连接请求，其他节点接收到连接请求后返回自己，保存节点拓扑  
  
  监控节点健康程度（每个一段时间互相发送对方的节点列表，根据节点列表重新发送连接请求）  
  监控到来的连接请求  
  
* 接口:更新链接请求（链接断开尝试手动重连）  
## 2. 命令处理
  接收来自本地节点应用层的命令请求（请求包括本地部署、远程部署、本地执行、远端执行、管理操作）  
  * Note:暂时不考虑本地节点应用层直接向远端ProcessListerer发送请求  
  本地部署则调用LocalDeployer实现部署  
  本地执行则调用LocalExecutor实现执行  
  远端部署则将消息转发给对应节点  
  (Plan 2)远端部署与执行使用RPC实现
  * 调用RemoteDeployer实现部署功能
  * 调用RemoteExecutor实现执行功能
## 3. 资源监控
  监控计算资源  
  管理计算资源  
  监控节点链路  

# 实现笔记
ProcessListener在系统启动后以服务形式执行
ProcessListener以守护进程形式存在于系统中
使用systemctl配置服务
服务文件所在目录为/lib/systemd/system/
程序执行时间周期内pid文件所在目录为/run/ProcessListener.pid
服务日志文件所在目录为/var/log/ProcessListener.log
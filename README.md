## Yggdrasill project

车载分布式系统构建计划

# 项目目标
* 解耦合
* 快速部署
* 简化执行过程

# 任务划分
任务细节内容详见doc文档。
## 1. 中间层
  * ProcessListener
    - deamon
    - 接受远程节点的执行命令
    - 调用Deployer实现部署功能
    - 调用Executor实现执行功能
    - 监控计算资源
    - 管理计算资源
    - 监控节点链路
  * Deployer
    - 部署程序功能
    - NFS实现
    - GIT实现
    - Docker实现
  * Executor
    - 程序执行程序
    - 进程管理
## 2. 数据传输层
  * 抽象数据传输层DataTransform
  * SocketLinker
    - Socket跨平台实现
  * SerialLinker
    - 串口跨平台实现
  * CanLinker(optional)
    - CAN跨平台实现
  * 开放基层接口，用于传感器传输
## 3. 持久化存储(Persistence)
  * FileInterface
    - 文件存储接口族
  * RedisInterface
    - Redis存储接口
  * DatabaseInterface
    - 数据库接口
## 4. 应用层
  * 慎用设计模式
  * 接纳算法
  * 接纳图形界面控制台
  * 接纳文本控制台
  * 接纳其他各种项目需求(dirty code)

## 开发过程中临时借用的第三方库
* tclap
* jsoncpp

## Ubuntu系统下安装以来环境
sudo apt install libtclap-dev libjsoncpp-dev
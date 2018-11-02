## DataTransform

任务：抽象数据传输层
# 原理说明
底层使用封装后的Socket\Serial\Can原始数据流。  
将数据传输统一为Send和Recv，建立连接的监听方作为Server，发起方作为Client。

# 关于Protocol定义
* 决定通信由哪个程序发起以及响应时间何时产生

# 关于Server/Client定义
* Server: 一般只响应客户端请求，仅在复杂的网络通信中使用异步回调（由服务器向客户发起请求）
* Client: 一般通信由客户发起请求

# 关于Serial类定义
* Serial类封装RS232接口数据读写操作
* 支持Linux操作系统
* 支持Windows操作系统
* 接口定义细节
  1. 打开端口
  2. 配置端口
  3. 发送数据（写）
  4. 接收数据（读）
  5. 关闭端口
* 接口实现注意事项
  1. 通讯错误抛出异常（暂无解决方案）
     * 打开错误
     * 发送错误（EIO：DCD信号线断开）
     * 读取错误（超时/错误）
  2. 配置端口
     * 串口名称
     * 波特率
     * 格式
     * 是否阻塞
     * 预留其他备选配置
  3. 发送数据
     * 发送结果检查
  4. 接收数据
     * 接收结果检查与校验
     * 完整性拼接

# SerialConfig.json配置文件定义

```
{
  "serialName" : "/dev/ttyS0",
  "speed" : 2400,
  "databits" : 8,
  "parity" : "N",
  "stopbits" : 1,
  "isBlocked" : false
}
```
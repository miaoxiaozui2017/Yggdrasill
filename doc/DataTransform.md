## DataTransform

任务：抽象数据传输层
# 原理说明
底层使用封装后的Socket\Serial\Can原始数据流。  
将数据传输统一为Send和Recv，建立连接的监听方作为Server，发起方作为Client。
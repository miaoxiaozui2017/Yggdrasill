## DevPlan

# 开发方案

* 该项目建立多个分支对应开发不同的层
* Ruler对应中间层，包括ProcessListner\Deployer\Executor
* Rider对应数据连接层，包括DataTransform\Socket\Serial
* Saber对应应用层，初期不适用
* Caster对应持久化存储，包括Redis\File\Database(other)

# 2018.11.06开发日志
* 根据UNIX网络编程书籍中包裹函数的定义，在封装Socket接口时引入错误处理函数族，在开发中用于错误信息输出、错误代码位置定位和日志信息重定向。
* 工具集内加入Log类和几种设计模式代码参考

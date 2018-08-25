## DevPlan

# 开发方案

* 该项目建立多个分支对应开发不同的层
* Ruler对应中间层，包括ProcessListner\Deployer\Executor
* Rider对应数据连接层，包括DataTransform\Socket\Serial
* Saber对应应用层，初期不适用
* Caster对应持久化存储，包括Redis\File\Database(other)
## 写行为树action/service子节点的步骤

以service为例

在sentry_interfaces/srv里自定义接口

在cmake里添加该接口并编译

在sentry_behavior_tree/include/sentry_behavior_tree/plugins/action里写该节点hpp文件

在sentry_behavior_tree/plugins/action里写该节点cpp文件  并在末尾注册节点

在cmake里添加该节点并编译

思考要不要改package.xml



注：节点里相当于client  can_comm_node.cpp里相当于server

在sentry_comm/include/sentry_comm/comm_node.hpp里增加callbackgroup  callback  server等

在sentry_comm/src/can_comm_node.cpp里写收到request的callback

编译can的cmake

思考要不要改package.xml



用Groot或其他方法得到只有一个该节点的行为树保存在sentry_bt_executor/behavior_trees下

修改sentry_bt_executor/src/bt_executor.cpp内的行为树的路径

新节点添加进plugin_libs

编译bt_executor.cpp

思考要不要改package.xml

运行并运行该树  然后launch can node  测试节点


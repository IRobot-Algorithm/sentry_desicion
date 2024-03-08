使用中：
使用launch文件启动
source install/setup.bash
ros2 launch sentry_comm sentry_comm.launch.py

已废弃:
使用方法
打开一个终端source install/setup.bash
ros2 run rclcpp_components component_container
打开第二个终端source install/setup.bash
ros2 component list  出现/ComponentManager
ros2 component load /ComponentManager sentry_comm sentry_comm::CanCommNode

若第二个终端ros2 component list  出现
/ComponentManager
  1  /sentry_comm
则为成功


其他命令：ros2 component types里面要有
sentry_comm
  sentry_comm::CanCommNode


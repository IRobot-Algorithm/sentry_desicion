source install/setup.bash
ros2 launch bringup start.launch.py

与ros1间通讯方法
第一个终端：开ros1节点

第二个终端：
source /opt/ros/noetic/setup
source /opt/ros/foxy/setup.bash
ros2 run ros1_bridge dynamic_bridge --bridge-all-topics

第三个终端：ros2订阅

第四个终端：
rosrun topic_tools transform /cmd_vel /cmd_vel geometry_msgs/Twist 'm.twist'




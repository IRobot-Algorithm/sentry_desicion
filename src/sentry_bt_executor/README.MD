source install/setup.bash


已废弃：
第一个终端：
source install/setup.bash
ros2 launch sentry_bt_executor bt_executor.launch.py
第二个终端：
ros2 launch sentry_comm sentry_comm.launch.py
ros2 topic echo /referee_information
ros2 topic echo path_index
第三个终端：
ros2 lifecycle get
ros2 lifecycle set /bt_executor configure
ros2 lifecycle set /bt_executor activate

不推荐使用：
ros2 run sentry_bt_executor bt_executor --ros-args --params-file ./src/sentry_bt_executor/config/bt_executor.yaml

import os
from launch import LaunchDescription
from launch_ros.actions import Node
from ament_index_python import get_package_share_directory


# 定义函数名称为：generate_launch_description
def generate_launch_description():

    communicate_node_ = Node(
        package='sentry_comm',
        namespace= '',
        executable='sentry_can',
        name='sentry_can',
        output='screen',
    )
    

    return LaunchDescription([communicate_node_])

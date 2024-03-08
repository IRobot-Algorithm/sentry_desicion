import os
from launch import LaunchDescription
from launch_ros.actions import Node
from ament_index_python import get_package_share_directory

#用于平时测试
# 定义函数名称为：generate_launch_description
def generate_launch_description():

    communicate_node_ = Node(
        package='sentry_comm',
        namespace= '',
        executable='sentry_can',
        name='sentry_can',
        output='screen',
    )

    information_processor_node_ = Node(
        package='sentry_bt_executor',
        namespace= '',
        executable='information_processor',
        name='information_processor',
        output='screen',
    )

    bt_executor_node_ = Node(
        package='sentry_bt_executor',
        namespace= '',
        executable='bt_executor',
        name='bt_executor',
        parameters=[{'default_bt_xml_filename': os.path.join(get_package_share_directory('sentry_bt_executor'),'behavior_trees/autoaim_patrol.xml')}],
        output='screen',
    )

    return LaunchDescription([communicate_node_,information_processor_node_,bt_executor_node_])


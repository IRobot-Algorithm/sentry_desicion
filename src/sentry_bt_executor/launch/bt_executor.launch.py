import os
from launch import LaunchDescription
from launch_ros.actions import Node
from ament_index_python import get_package_share_directory


# 定义函数名称为：generate_launch_description
def generate_launch_description():

    # information_processor_node_ = Node(
    #     package='sentry_bt_executor',
    #     namespace= '',
    #     executable='information_processor',
    #     name='information_processor',
    #     output='screen',
    # )

    bt_executor_node_ = Node(
        package='sentry_bt_executor',
        namespace= '',
        executable='bt_executor',
        name='bt_executor',
        #注意树的名称
        parameters=[{'default_bt_xml_filename': os.path.join(get_package_share_directory('sentry_bt_executor'), \
                     'behavior_trees/test_rmul.xml')}],
        output='screen',
    )
    # bt_executor_node_ = Node(
    #     package='sentry_bt_executor',
    #     namespace= '',m
    #     name='bt_executor',
    #     #注意树的名称
    #     parameters=[{'default_bt_xml_filename': os.path.join(get_package_share_directory('sentry_bt_executor'),'behavior_trees/new_rmuc_patrol_tree.xml')}],
    #     output='screen',
    # ) 
    # 
    #Node里面一定要有output='screen'!!
    #    

    return LaunchDescription([bt_executor_node_])

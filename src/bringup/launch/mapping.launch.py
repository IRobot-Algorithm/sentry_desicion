from ament_index_python.packages import get_package_share_directory
from launch import LaunchDescription
from launch.actions import IncludeLaunchDescription
from launch.launch_description_sources import PythonLaunchDescriptionSource
from launch.substitutions import LaunchConfiguration
from launch_ros.actions import Node
import os
import launch
from launch_ros.actions import ComposableNodeContainer
from launch_ros.descriptions import ComposableNode

# 定义函数名称为：generate_launch_description
def generate_launch_description():

    fishbot_nav2_dir = get_package_share_directory('fishbot_navigation2')

    fishbot_test_launch = IncludeLaunchDescription(
            # PythonLaunchDescriptionSource([test_dir,'/launch','/view2_robot_launch.py']),
            PythonLaunchDescriptionSource(
        	os.path.join(
        		get_package_share_directory('test_urdf'),
        		'launch/view2_robot_launch.py'
        	)
        )
        )
    t265_node =  Node(
            package='realsense_ros2',
            executable='rs_t265_node',
            name='rs_t265_node',
            output='screen')
    t265_tf_node=Node(
            package='tf2_ros',
            executable='static_transform_publisher',
            arguments = ['-0.180','0.000', '-0.400', '0.000', '0.000', '-0.707', '0.707', '/t265_frame', '/base_footprint']
    )

    

    
    

    return LaunchDescription([fishbot_test_launch,t265_node,t265_tf_node])
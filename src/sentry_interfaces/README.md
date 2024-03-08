``` 
cwl@cwl-cpt:~/Desktop/desicion_ws$ ros2 interface show geometry_msgs/msg/PointStamped

# This represents a Point with reference coordinate frame and timestamp

std_msgs/Header header
Point point
```



编写自定义接口PatrolState.srv时要将Point point改为geometry_msgs/Point point
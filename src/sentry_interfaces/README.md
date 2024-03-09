## FollowTarget.msg

- geometry_msgs/PointStamped target

目标位置，注意时间戳为相机采样时间戳

- uint8 have_target
  - 0 : 没有目标
  - 1:在跟踪目标
  - 2:丢失目标 但tracker还在
  
- uint8 priority
  - 目标优先级
  
    如我这边的排序为英雄 哨兵 基地 步兵3 其他不打
  
    则会发 1, 6, 0, 3

## AimTarget.srv

- uint8[] list

  - 0 基地
  - 1 英雄
  - 2 工程
  - 3 4 5 步兵
  - 6 哨兵
  - 7 前哨站

- bool success

  服务标识符 先直接success = true 后面出问题再改
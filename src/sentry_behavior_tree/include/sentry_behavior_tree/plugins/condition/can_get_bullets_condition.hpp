#ifndef SENTRY_BEHAVIOR_TREE__PLUGINS__CONDITION__CAN_GET_BULLETS_CONDITION_HPP_
#define SENTRY_BEHAVIOR_TREE__PLUGINS__CONDITION__CAN_GET_BULLETS_CONDITION_HPP_

#include "behaviortree_cpp_v3/behavior_tree.h"

namespace sentry_behavior_tree
{
/*
* @brief 判断是否可以领取子弹
* @auther wyq
*/
BT::NodeStatus canGetBullets(BT::TreeNode & tree_node);

}

#endif
#ifndef SENTRY_BEHAVIOR_TREE__PLUGINS__CONDITION__CAN_BUY_BULLETS_CONDITION_HPP_
#define SENTRY_BEHAVIOR_TREE__PLUGINS__CONDITION__CAN_BUY_BULLETS_CONDITION_HPP_

#include "behaviortree_cpp_v3/behavior_tree.h"

namespace sentry_behavior_tree
{
/*
* @brief 判断是否能买弹
* @auther wyq
*/
BT::NodeStatus canBuyBullets(BT::TreeNode & tree_node);

}

#endif
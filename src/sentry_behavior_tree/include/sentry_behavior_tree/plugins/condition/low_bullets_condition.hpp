#ifndef SENTRY_BEHAVIOR_TREE__PLUGINS__CONDITION__LOW_BULLETS_CONDITION_HPP_
#define SENTRY_BEHAVIOR_TREE__PLUGINS__CONDITION__LOW_BULLETS_CONDITION_HPP_

#include "behaviortree_cpp_v3/behavior_tree.h"

namespace sentry_behavior_tree
{
/*
* @brief 判断哨兵是否有弹丸
* @auther wyq
*/
BT::NodeStatus lowBullets(BT::TreeNode & tree_node);

}

#endif
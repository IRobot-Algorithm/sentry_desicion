#ifndef SENTRY_BEHAVIOR_TREE__PLUGINS__CONDITION__LOW_HP_CONDITION_HPP_
#define SENTRY_BEHAVIOR_TREE__PLUGINS__CONDITION__LOW_HP_CONDITION_HPP_

#include "behaviortree_cpp_v3/behavior_tree.h"

namespace sentry_behavior_tree
{
/*
* @brief 判断哨兵是否血量过低
* @auther wyq
*/
BT::NodeStatus lowHp(BT::TreeNode & tree_node);

}

#endif
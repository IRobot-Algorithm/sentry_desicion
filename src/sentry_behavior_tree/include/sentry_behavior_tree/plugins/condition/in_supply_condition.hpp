#ifndef SENTRY_BEHAVIOR_TREE__PLUGINS__CONDITION__IN_SUPPLY_CONDITION_HPP_
#define SENTRY_BEHAVIOR_TREE__PLUGINS__CONDITION__IN_SUPPLY_CONDITION_HPP_

#include "behaviortree_cpp_v3/behavior_tree.h"

namespace sentry_behavior_tree
{
/*
* @brief 判断哨兵是否在补给区
* @auther wyq
*/
BT::NodeStatus inSupply(BT::TreeNode & tree_node);

}

#endif
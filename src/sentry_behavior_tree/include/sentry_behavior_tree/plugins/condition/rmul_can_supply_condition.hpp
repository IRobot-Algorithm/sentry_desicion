#ifndef SENTRY_BEHAVIOR_TREE__PLUGINS__CONDITION__RMUL_CAN_SUPPLY_CONDITION_HPP_
#define SENTRY_BEHAVIOR_TREE__PLUGINS__CONDITION__RMUL_CAN_SUPPLY_CONDITION_HPP_

#include "behaviortree_cpp_v3/behavior_tree.h"

namespace sentry_behavior_tree
{
/*
* @brief rmul判断是否能进行补给
* @auther wyq
*/
BT::NodeStatus rmulCanSupply(BT::TreeNode & tree_node);

}

#endif
#ifndef SENTRY_BEHAVIOR_TREE__PLUGINS__CONDITION__COUNTER_ATTACK_CONDITION_HPP_
#define SENTRY_BEHAVIOR_TREE__PLUGINS__CONDITION__COUNTER_ATTACK_CONDITION_HPP_

#include "behaviortree_cpp_v3/behavior_tree.h"

namespace sentry_behavior_tree
{
/*
* @brief 基地展开大符激活后反击
* @auther wyq
*/
BT::NodeStatus counterAttack(BT::TreeNode & tree_node);

}

#endif
#ifndef SENTRY_BEHAVIOR_TREE__PLUGINS__CONDITION__ENOUGH_HP_CONDITION_HPP_
#define SENTRY_BEHAVIOR_TREE__PLUGINS__CONDITION__ENOUGH_HP_CONDITION_HPP_

#include "behaviortree_cpp_v3/behavior_tree.h"

namespace sentry_behavior_tree
{
/*
* @brief 判断哨兵是否血量足够
* @auther wyq
*/
BT::NodeStatus enoughHp(BT::TreeNode & tree_node);

}

#endif
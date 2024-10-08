#ifndef SENTRY_BEHAVIOR_TREE__PLUGINS__CONDITION__OUTPOST_LOW_HP_CONDITION_HPP_
#define SENTRY_BEHAVIOR_TREE__PLUGINS__CONDITION__OUTPOST_LOW_HP_CONDITION_HPP_

#include "behaviortree_cpp_v3/behavior_tree.h"

namespace sentry_behavior_tree
{
/*
* @brief 判断我方前哨站是否血量低
* @auther wyq
*/
BT::NodeStatus outpostLowHp(BT::TreeNode & tree_node);

}

#endif
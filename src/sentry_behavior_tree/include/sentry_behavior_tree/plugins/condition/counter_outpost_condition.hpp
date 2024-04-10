#ifndef SENTRY_BEHAVIOR_TREE__PLUGINS__CONDITION__COUNTER_OUTPOST_CONDITION_HPP_
#define SENTRY_BEHAVIOR_TREE__PLUGINS__CONDITION__COUNTER_OUTPOST_CONDITION_HPP_

#include "behaviortree_cpp_v3/behavior_tree.h"

namespace sentry_behavior_tree
{
/*
* @brief 判断是否是反前哨站模式
* @auther wyq
*/
BT::NodeStatus counterOutpost(BT::TreeNode & tree_node);

}

#endif
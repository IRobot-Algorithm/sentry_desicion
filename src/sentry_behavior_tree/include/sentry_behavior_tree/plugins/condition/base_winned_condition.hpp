#ifndef SENTRY_BEHAVIOR_TREE__PLUGINS__CONDITION__BASE_WINNED_CONDITION_HPP_
#define SENTRY_BEHAVIOR_TREE__PLUGINS__CONDITION__BASE_WINNED_CONDITION_HPP_

#include "behaviortree_cpp_v3/behavior_tree.h"

namespace sentry_behavior_tree
{
/*
* @brief 判断我方基地血量比敌方少
* @auther wyq
*/
BT::NodeStatus baseWinned(BT::TreeNode & tree_node);

}

#endif
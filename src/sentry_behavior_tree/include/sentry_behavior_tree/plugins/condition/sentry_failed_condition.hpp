#ifndef SENTRY_BEHAVIOR_TREE__PLUGINS__CONDITION__SENTRY_FAILED_CONDITION_HPP_
#define SENTRY_BEHAVIOR_TREE__PLUGINS__CONDITION__SENTRY_FAILED_CONDITION_HPP_

#include "behaviortree_cpp_v3/behavior_tree.h"

namespace sentry_behavior_tree
{
/*
* @brief 判断我方因哨兵血量少失败
* @auther wyq
*/
BT::NodeStatus sentryFailed(BT::TreeNode & tree_node);

}

#endif
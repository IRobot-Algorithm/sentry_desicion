#ifndef SENTRY_BEHAVIOR_TREE__PLUGINS__CONDITION__FORCE_BACK_CONDITION_HPP_
#define SENTRY_BEHAVIOR_TREE__PLUGINS__CONDITION__FORCE_BACK_CONDITION_HPP_

#include "behaviortree_cpp_v3/behavior_tree.h"

namespace sentry_behavior_tree
{
/*
* @brief 云台手指令 强制回家
* @auther wyq
*/
BT::NodeStatus forceBack(BT::TreeNode & tree_node);

}

#endif
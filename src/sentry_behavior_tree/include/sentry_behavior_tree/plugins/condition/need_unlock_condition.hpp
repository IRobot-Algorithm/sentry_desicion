#ifndef SENTRY_BEHAVIOR_TREE__PLUGINS__CONDITION__NEED_UNLOCK_CONDITION_HPP_
#define SENTRY_BEHAVIOR_TREE__PLUGINS__CONDITION__NEED_UNLOCK_CONDITION_HPP_

#include "behaviortree_cpp_v3/behavior_tree.h"

namespace sentry_behavior_tree
{
/*
* @brief 判断是否需要解锁发射机构
* @auther wyq
*/
BT::NodeStatus needUnlock(BT::TreeNode & tree_node);

}

#endif
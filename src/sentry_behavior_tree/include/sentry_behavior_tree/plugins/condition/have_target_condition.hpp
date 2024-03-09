#ifndef SENTRY_BEHAVIOR_TREE__PLUGINS__CONDITION__HAVE_TARGET_CONDITION_HPP_
#define SENTRY_BEHAVIOR_TREE__PLUGINS__CONDITION__HAVE_TARGET_CONDITION_HPP_

#include "behaviortree_cpp_v3/behavior_tree.h"

namespace sentry_behavior_tree
{
/*
* @brief 判断是否有自瞄目标
* @auther wyq
*/
BT::NodeStatus haveTarget(BT::TreeNode & tree_node);

}

#endif
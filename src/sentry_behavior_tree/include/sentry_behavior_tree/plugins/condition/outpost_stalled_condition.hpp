#ifndef SENTRY_BEHAVIOR_TREE__PLUGINS__CONDITION__OUTPOST_STALLED_CONDITION_HPP_
#define SENTRY_BEHAVIOR_TREE__PLUGINS__CONDITION__OUTPOST_STALLED_CONDITION_HPP_

#include "behaviortree_cpp_v3/behavior_tree.h"

namespace sentry_behavior_tree
{
/*
* @brief 判断前哨站是否停转
* @auther wyq
*/
BT::NodeStatus outpostStalled(BT::TreeNode & tree_node);

}

#endif
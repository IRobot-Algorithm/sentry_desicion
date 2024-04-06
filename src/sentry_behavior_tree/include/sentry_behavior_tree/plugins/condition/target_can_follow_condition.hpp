#ifndef SENTRY_BEHAVIOR_TREE__PLUGINS__CONDITION__TARGET_CAN_FOLLOW_CONDITION_HPP_
#define SENTRY_BEHAVIOR_TREE__PLUGINS__CONDITION__TARGET_CAN_FOLLOW_CONDITION_HPP_

#include "behaviortree_cpp_v3/behavior_tree.h"

namespace sentry_behavior_tree
{
/*
* @brief 判断是否能进行跟随
* @auther wyq
*/
BT::NodeStatus targetCanFollow(BT::TreeNode & tree_node);

}

#endif
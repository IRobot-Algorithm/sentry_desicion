#ifndef SENTRY_BEHAVIOR_TREE__PLUGINS__CONDITION__LEAVE_TOO_LONG_CONDITION_HPP_
#define SENTRY_BEHAVIOR_TREE__PLUGINS__CONDITION__LEAVE_TOO_LONG_CONDITION_HPP_

#include "behaviortree_cpp_v3/behavior_tree.h"

namespace sentry_behavior_tree
{
/*
* @brief 判断哨兵是否离开巡逻区过久
* @auther wyq
*/
BT::NodeStatus leaveTooLong(BT::TreeNode & tree_node);

}

#endif
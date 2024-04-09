#ifndef SENTRY_BEHAVIOR_TREE__PLUGINS__CONDITION__IN_PATROL_CONDITION_HPP_
#define SENTRY_BEHAVIOR_TREE__PLUGINS__CONDITION__IN_PATROL_CONDITION_HPP_

#include "behaviortree_cpp_v3/behavior_tree.h"

namespace sentry_behavior_tree
{
/*
* @brief 判断哨兵是否在巡逻区
* @auther wyq
*/
BT::NodeStatus inPatrol(BT::TreeNode & tree_node);

}

#endif
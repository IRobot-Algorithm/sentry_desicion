#ifndef SENTRY_BEHAVIOR_TREE__PLUGINS__CONDITION__CENTER_PATROL_CONDITION_HPP_
#define SENTRY_BEHAVIOR_TREE__PLUGINS__CONDITION__CENTER_PATROL_CONDITION_HPP_

#include "behaviortree_cpp_v3/behavior_tree.h"

namespace sentry_behavior_tree
{
/*
* @brief 判断是否是中心点巡逻模式
* @auther wyq
*/
BT::NodeStatus centerPatrol(BT::TreeNode & tree_node);

}

#endif
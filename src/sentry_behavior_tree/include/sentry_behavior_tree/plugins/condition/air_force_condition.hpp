#ifndef SENTRY_BEHAVIOR_TREE__PLUGINS__CONDITION__AIR_FORCE_CONDITION_HPP_
#define SENTRY_BEHAVIOR_TREE__PLUGINS__CONDITION__AIR_FORCE_CONDITION_HPP_

#include "behaviortree_cpp_v3/behavior_tree.h"

namespace sentry_behavior_tree
{
/*
* @brief 判断敌方无人机是否起飞
* @auther wyq
*/
BT::NodeStatus airForce(BT::TreeNode & tree_node);

}

#endif
#ifndef SENTRY_BEHAVIOR_TREE__PLUGINS__CONDITION__ENEMY_AREA_PATROL_CONDITION_HPP_
#define SENTRY_BEHAVIOR_TREE__PLUGINS__CONDITION__ENEMY_AREA_PATROL_CONDITION_HPP_

#include "behaviortree_cpp_v3/behavior_tree.h"

namespace sentry_behavior_tree
{
/*
* @brief 判断是否是敌方区域巡逻模式
* @auther wyq
*/
BT::NodeStatus enemyAreaPatrol(BT::TreeNode & tree_node);

}

#endif
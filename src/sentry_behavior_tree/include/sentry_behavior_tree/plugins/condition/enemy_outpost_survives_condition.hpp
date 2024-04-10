#ifndef SENTRY_BEHAVIOR_TREE__PLUGINS__CONDITION__ENEMY_OUTPOST_SURVIVES_CONDITION_HPP_
#define SENTRY_BEHAVIOR_TREE__PLUGINS__CONDITION__ENEMY_OUTPOST_SURVIVES_CONDITION_HPP_

#include "behaviortree_cpp_v3/behavior_tree.h"

namespace sentry_behavior_tree
{
/*
* @brief 判断敌方前哨站是否存活
* @auther wyq
*/
BT::NodeStatus EnemyOutpostSurvives(BT::TreeNode & tree_node);

}

#endif
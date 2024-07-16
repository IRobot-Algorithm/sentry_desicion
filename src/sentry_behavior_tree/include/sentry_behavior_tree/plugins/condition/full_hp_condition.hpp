#ifndef SENTRY_BEHAVIOR_TREE__PLUGINS__CONDITION__FULL_HP_CONDITION_HPP_
#define SENTRY_BEHAVIOR_TREE__PLUGINS__CONDITION__FULL_HP_CONDITION_HPP_

#include "behaviortree_cpp_v3/behavior_tree.h"

namespace sentry_behavior_tree
{
/*
* @brief 判断哨兵是否manxue
* @auther wyq
*/
BT::NodeStatus fullHp(BT::TreeNode & tree_node);

}

#endif
#ifndef SENTRY_BEHAVIOR_TREE__PLUGINS__CONDITION__GAME_START_CONDITION_HPP_
#define SENTRY_BEHAVIOR_TREE__PLUGINS__CONDITION__GAME_START_CONDITION_HPP_

#include "behaviortree_cpp_v3/behavior_tree.h"

namespace sentry_behavior_tree
{
/*
* @brief 判断游戏是否开始
* @auther wyq
*/
BT::NodeStatus gameStart(BT::TreeNode & tree_node);

}

#endif
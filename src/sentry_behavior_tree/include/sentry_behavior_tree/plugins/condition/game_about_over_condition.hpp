#ifndef SENTRY_BEHAVIOR_TREE__PLUGINS__CONDITION__GAME_ABOUT_OVER_CONDITION_HPP_
#define SENTRY_BEHAVIOR_TREE__PLUGINS__CONDITION__GAME_ABOUT_OVER_CONDITION_HPP_

#include "behaviortree_cpp_v3/behavior_tree.h"

namespace sentry_behavior_tree
{
/*
* @brief 判断游戏是否即将结束
* @auther wyq
*/
BT::NodeStatus gameAboutOver(BT::TreeNode & tree_node);

}

#endif
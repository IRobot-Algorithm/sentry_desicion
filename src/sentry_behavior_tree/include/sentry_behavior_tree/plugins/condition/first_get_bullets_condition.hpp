#ifndef SENTRY_BEHAVIOR_TREE__PLUGINS__CONDITION__FIRST_GET_BULLETS_CONDITION_HPP_
#define SENTRY_BEHAVIOR_TREE__PLUGINS__CONDITION__FIRST_GET_BULLETS_CONDITION_HPP_

#include "behaviortree_cpp_v3/behavior_tree.h"

namespace sentry_behavior_tree
{
/*
* @brief 判断是否第一次领过子弹
* @auther wyq
*/
BT::NodeStatus firstGetBullets(BT::TreeNode & tree_node);

}

#endif
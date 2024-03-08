#ifndef SENTRY_BEHAVIOR_TREE__PLUGINS__CONDITION__BASE_UNFOLDS_CONDITION_HPP_
#define SENTRY_BEHAVIOR_TREE__PLUGINS__CONDITION__BASE_UNFOLDS_CONDITION_HPP_

#include "behaviortree_cpp_v3/behavior_tree.h"

namespace sentry_behavior_tree
{
/*
* @brief 判断我方基地装甲板是否展开
* @auther wyq
*/
BT::NodeStatus baseUnfolds(BT::TreeNode & tree_node);

}

#endif
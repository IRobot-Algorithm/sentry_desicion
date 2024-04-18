#include "sentry_behavior_tree/plugins/condition/need_unlock_condition.hpp"
#include "math.h"
namespace sentry_behavior_tree
{

    BT::NodeStatus needUnlock(BT::TreeNode & tree_node){

        auto need_unlock = tree_node.config().blackboard->get<bool>("need_unlock");

        return need_unlock ? BT::NodeStatus::SUCCESS : BT::NodeStatus::FAILURE;

    }


}

#include "behaviortree_cpp_v3/bt_factory.h"

BT_REGISTER_NODES(factory)
{
  factory.registerSimpleCondition(
    "needUnlock",
    std::bind(&sentry_behavior_tree::needUnlock, std::placeholders::_1));
}



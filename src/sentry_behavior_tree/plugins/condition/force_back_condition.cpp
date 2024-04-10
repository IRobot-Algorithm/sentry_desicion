#include "sentry_behavior_tree/plugins/condition/force_back_condition.hpp"
#include "math.h"
namespace sentry_behavior_tree
{

    BT::NodeStatus forceBack(BT::TreeNode & tree_node){

        auto force_back = tree_node.config().blackboard->get<bool>("force_back");

        return force_back ? BT::NodeStatus::SUCCESS : BT::NodeStatus::FAILURE;

    }


}

#include "behaviortree_cpp_v3/bt_factory.h"

BT_REGISTER_NODES(factory)
{
  factory.registerSimpleCondition(
    "forceBack",
    std::bind(&sentry_behavior_tree::forceBack, std::placeholders::_1));
}



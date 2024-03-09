#include "sentry_behavior_tree/plugins/condition/base_unfolds_condition.hpp"
#include "math.h"
namespace sentry_behavior_tree
{


    BT::NodeStatus baseUnfolds(BT::TreeNode & tree_node){

        auto base_shield = tree_node.config().blackboard->get<u_int8_t>("base_shield");

        return base_shield <= 0 ? BT::NodeStatus::SUCCESS : BT::NodeStatus::FAILURE;

    }


}

#include "behaviortree_cpp_v3/bt_factory.h"

BT_REGISTER_NODES(factory)
{
  factory.registerSimpleCondition(
    "baseUnfolds",
    std::bind(&sentry_behavior_tree::baseUnfolds, std::placeholders::_1));
}



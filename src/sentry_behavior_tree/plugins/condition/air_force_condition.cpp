#include "sentry_behavior_tree/plugins/condition/air_force_condition.hpp"
#include "math.h"
namespace sentry_behavior_tree
{

    BT::NodeStatus airForce(BT::TreeNode & tree_node){

        auto air_force = tree_node.config().blackboard->get<bool>("air_force");

        return air_force ? BT::NodeStatus::SUCCESS : BT::NodeStatus::FAILURE;

    }


}

#include "behaviortree_cpp_v3/bt_factory.h"

BT_REGISTER_NODES(factory)
{
  factory.registerSimpleCondition(
    "airForce",
    std::bind(&sentry_behavior_tree::airForce, std::placeholders::_1));
}



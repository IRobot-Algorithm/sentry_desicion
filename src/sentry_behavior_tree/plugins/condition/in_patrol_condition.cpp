#include "sentry_behavior_tree/plugins/condition/in_patrol_condition.hpp"
#include "math.h"
namespace sentry_behavior_tree
{


    BT::NodeStatus inPatrol(BT::TreeNode & tree_node){

        auto in_patrol = tree_node.config().blackboard->get<bool>("in_patrol");

        return in_patrol ? BT::NodeStatus::SUCCESS : BT::NodeStatus::FAILURE;


    }


}

#include "behaviortree_cpp_v3/bt_factory.h"

BT_REGISTER_NODES(factory)
{
  factory.registerSimpleCondition(
    "inPatrol",
    std::bind(&sentry_behavior_tree::inPatrol, std::placeholders::_1));
}



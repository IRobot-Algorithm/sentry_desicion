#include "sentry_behavior_tree/plugins/condition/in_supply_condition.hpp"
#include "math.h"
namespace sentry_behavior_tree
{


    BT::NodeStatus inSupply(BT::TreeNode & tree_node){

        auto in_supply = tree_node.config().blackboard->get<bool>("in_supply");

        return in_supply ? BT::NodeStatus::SUCCESS : BT::NodeStatus::FAILURE;


    }


}

#include "behaviortree_cpp_v3/bt_factory.h"

BT_REGISTER_NODES(factory)
{
  factory.registerSimpleCondition(
    "inSupply",
    std::bind(&sentry_behavior_tree::inSupply, std::placeholders::_1));
}



#include "sentry_behavior_tree/plugins/condition/rmul_can_supply_condition.hpp"
#include "math.h"
namespace sentry_behavior_tree
{


    BT::NodeStatus rmulCanSupply(BT::TreeNode & tree_node){

        auto supply_time = tree_node.config().blackboard->get<double>("supply_time");

        return supply_time < 10 ? BT::NodeStatus::SUCCESS : BT::NodeStatus::FAILURE;

    }


}

#include "behaviortree_cpp_v3/bt_factory.h"

BT_REGISTER_NODES(factory)
{
  factory.registerSimpleCondition(
    "rmulCanSupply",
    std::bind(&sentry_behavior_tree::rmulCanSupply, std::placeholders::_1));
}



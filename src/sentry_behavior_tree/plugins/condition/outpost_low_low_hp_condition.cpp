#include "sentry_behavior_tree/plugins/condition/outpost_low_low_hp_condition.hpp"
#include "math.h"
namespace sentry_behavior_tree
{


    BT::NodeStatus outpostLowLowHp(BT::TreeNode & tree_node){

        auto our_outpost_hp = tree_node.config().blackboard->get<u_int16_t>("our_outpost_hp");

        return our_outpost_hp <= 300 ? BT::NodeStatus::SUCCESS : BT::NodeStatus::FAILURE;

    }


}

#include "behaviortree_cpp_v3/bt_factory.h"

BT_REGISTER_NODES(factory)
{
  factory.registerSimpleCondition(
    "outpostLowLowHp",
    std::bind(&sentry_behavior_tree::outpostLowLowHp, std::placeholders::_1));
}



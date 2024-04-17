#include "sentry_behavior_tree/plugins/condition/can_buy_bullets_condition.hpp"
#include "math.h"
namespace sentry_behavior_tree
{


    BT::NodeStatus canBuyBullets(BT::TreeNode & tree_node){

        auto can_buy_bullets = tree_node.config().blackboard->get<bool>("can_buy_bullets");

        return can_buy_bullets ? BT::NodeStatus::SUCCESS : BT::NodeStatus::FAILURE;

    }


}

#include "behaviortree_cpp_v3/bt_factory.h"

BT_REGISTER_NODES(factory)
{
  factory.registerSimpleCondition(
    "canBuyBullets",
    std::bind(&sentry_behavior_tree::canBuyBullets, std::placeholders::_1));
}



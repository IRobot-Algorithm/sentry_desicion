#include "sentry_behavior_tree/plugins/condition/first_get_bullets_condition.hpp"
#include "math.h"
namespace sentry_behavior_tree
{

    BT::NodeStatus firstGetBullets(BT::TreeNode & tree_node){

        auto first_get_bullets = tree_node.config().blackboard->get<bool>("first_get_bullets");

        return first_get_bullets ? BT::NodeStatus::SUCCESS : BT::NodeStatus::FAILURE;

    }


}

#include "behaviortree_cpp_v3/bt_factory.h"

BT_REGISTER_NODES(factory)
{
  factory.registerSimpleCondition(
    "firstGetBullets",
    std::bind(&sentry_behavior_tree::firstGetBullets, std::placeholders::_1));
}



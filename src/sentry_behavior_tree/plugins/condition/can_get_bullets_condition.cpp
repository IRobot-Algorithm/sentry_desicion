#include "sentry_behavior_tree/plugins/condition/can_get_bullets_condition.hpp"
#include "math.h"
namespace sentry_behavior_tree
{


    BT::NodeStatus canGetBullets(BT::TreeNode & tree_node){

        auto can_get_bullets = tree_node.config().blackboard->get<double>("supply_time") > 60.0;

        return can_get_bullets ? BT::NodeStatus::SUCCESS : BT::NodeStatus::FAILURE;


    }


}

#include "behaviortree_cpp_v3/bt_factory.h"

BT_REGISTER_NODES(factory)
{
  factory.registerSimpleCondition(
    "canGetBullets",
    std::bind(&sentry_behavior_tree::canGetBullets, std::placeholders::_1));
}



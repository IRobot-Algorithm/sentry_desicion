#include "sentry_behavior_tree/plugins/condition/low_bullets_condition.hpp"
#include "math.h"
namespace sentry_behavior_tree
{


    BT::NodeStatus lowBullets(BT::TreeNode & tree_node){

        auto low_bullets = tree_node.config().blackboard->get<u_int16_t>("bullets") <= 0;

        return low_bullets ? BT::NodeStatus::SUCCESS : BT::NodeStatus::FAILURE;


    }


}

#include "behaviortree_cpp_v3/bt_factory.h"

BT_REGISTER_NODES(factory)
{
  factory.registerSimpleCondition(
    "lowBullets",
    std::bind(&sentry_behavior_tree::lowBullets, std::placeholders::_1));
}



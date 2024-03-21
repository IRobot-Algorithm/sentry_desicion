#include "sentry_behavior_tree/plugins/condition/enough_hp_condition.hpp"
#include "math.h"
namespace sentry_behavior_tree
{


    BT::NodeStatus enoughHp(BT::TreeNode & tree_node){

        auto enough_hp = tree_node.config().blackboard->get<u_int16_t>("robot_hp") >= 500;

        return enough_hp ? BT::NodeStatus::SUCCESS : BT::NodeStatus::FAILURE;


    }


}

#include "behaviortree_cpp_v3/bt_factory.h"

BT_REGISTER_NODES(factory)
{
  factory.registerSimpleCondition(
    "enoughHp",
    std::bind(&sentry_behavior_tree::enoughHp, std::placeholders::_1));
}



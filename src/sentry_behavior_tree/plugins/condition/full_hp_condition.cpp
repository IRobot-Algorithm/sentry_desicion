#include "sentry_behavior_tree/plugins/condition/full_hp_condition.hpp"
#include "math.h"
namespace sentry_behavior_tree
{


    BT::NodeStatus fullHp(BT::TreeNode & tree_node){

        auto full_hp = tree_node.config().blackboard->get<u_int16_t>("robot_hp") == 400;

        return full_hp ? BT::NodeStatus::SUCCESS : BT::NodeStatus::FAILURE;


    }


}

#include "behaviortree_cpp_v3/bt_factory.h"

BT_REGISTER_NODES(factory)
{
  factory.registerSimpleCondition(
    "fullHp",
    std::bind(&sentry_behavior_tree::fullHp, std::placeholders::_1));
}



#include "sentry_behavior_tree/plugins/condition/low_hp_condition.hpp"
#include "math.h"
namespace sentry_behavior_tree
{


    BT::NodeStatus lowHp(BT::TreeNode & tree_node){

        auto low_hp = tree_node.config().blackboard->get<u_int16_t>("robot_hp") <= 200;

        return low_hp ? BT::NodeStatus::SUCCESS : BT::NodeStatus::FAILURE;


    }


}

#include "behaviortree_cpp_v3/bt_factory.h"

BT_REGISTER_NODES(factory)
{
  factory.registerSimpleCondition(
    "lowHp",
    std::bind(&sentry_behavior_tree::lowHp, std::placeholders::_1));
}



#include "sentry_behavior_tree/plugins/condition/low_hp_condition.hpp"
#include "math.h"
namespace sentry_behavior_tree
{


    BT::NodeStatus lowHp(BT::TreeNode & tree_node){

        auto have_buff = tree_node.config().blackboard->get<bool>("have_buff");
        bool low_hp;
        if (have_buff)
            low_hp = tree_node.config().blackboard->get<u_int16_t>("robot_hp") <= 100;
        else
            low_hp = tree_node.config().blackboard->get<u_int16_t>("robot_hp") <= 150;

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



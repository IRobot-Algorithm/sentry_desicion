#include "sentry_behavior_tree/plugins/condition/leave_too_long_condition.hpp"
#include "math.h"
namespace sentry_behavior_tree
{


    BT::NodeStatus leaveTooLong(BT::TreeNode & tree_node){

        auto leave_time = tree_node.config().blackboard->get<double>("leave_time");
        auto now_time = tree_node.config().blackboard->get<double>("now_time");
        auto gameover_time = tree_node.config().blackboard->get<u_int16_t>("gameover_time");
        auto long_time = gameover_time > 180 ? 45 : 15;

        // std::cout << now_time << " " << leave_time << std::endl;
        return now_time - leave_time > long_time ? BT::NodeStatus::SUCCESS : BT::NodeStatus::FAILURE;
    }


}

#include "behaviortree_cpp_v3/bt_factory.h"

BT_REGISTER_NODES(factory)
{
  factory.registerSimpleCondition(
    "leaveTooLong",
    std::bind(&sentry_behavior_tree::leaveTooLong, std::placeholders::_1));
}



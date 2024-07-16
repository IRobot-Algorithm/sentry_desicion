#include "sentry_behavior_tree/plugins/condition/need_avoid_condition.hpp"
#include "math.h"
namespace sentry_behavior_tree
{

    BT::NodeStatus needAvoid(BT::TreeNode & tree_node){

        auto now_time = tree_node.config().blackboard->get<double>("now_time");
        auto hit_time = tree_node.config().blackboard->get<double>("hit_time");
        auto robot_hp = tree_node.config().blackboard->get<u_int16_t>("robot_hp");

        return (now_time - hit_time < 1.5 && robot_hp < 200)
                ? BT::NodeStatus::SUCCESS : BT::NodeStatus::FAILURE;

    }


}

#include "behaviortree_cpp_v3/bt_factory.h"

BT_REGISTER_NODES(factory)
{
  factory.registerSimpleCondition(
    "needAvoid",
    std::bind(&sentry_behavior_tree::needAvoid, std::placeholders::_1));
}



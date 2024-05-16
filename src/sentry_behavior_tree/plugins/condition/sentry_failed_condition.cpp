#include "sentry_behavior_tree/plugins/condition/sentry_failed_condition.hpp"
#include "math.h"
namespace sentry_behavior_tree
{


    BT::NodeStatus sentryFailed(BT::TreeNode & tree_node){

        auto base_same = tree_node.config().blackboard->get<u_int16_t>("our_base_hp") ==
                         tree_node.config().blackboard->get<u_int16_t>("base_hp");

        auto sentry_failed = tree_node.config().blackboard->get<u_int16_t>("robot_hp") <
                             tree_node.config().blackboard->get<u_int16_t>("sentry_hp");

        return (base_same && sentry_failed) ? BT::NodeStatus::SUCCESS : BT::NodeStatus::FAILURE;


    }


}

#include "behaviortree_cpp_v3/bt_factory.h"

BT_REGISTER_NODES(factory)
{
  factory.registerSimpleCondition(
    "sentryFailed",
    std::bind(&sentry_behavior_tree::sentryFailed, std::placeholders::_1));
}



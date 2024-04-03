#include "sentry_behavior_tree/plugins/condition/base_failed_condition.hpp"
#include "math.h"
namespace sentry_behavior_tree
{


    BT::NodeStatus baseFailed(BT::TreeNode & tree_node){

        auto base_failed = tree_node.config().blackboard->get<u_int16_t>("our_base_hp") < 
                           tree_node.config().blackboard->get<u_int16_t>("base_hp");

        return base_failed ? BT::NodeStatus::SUCCESS : BT::NodeStatus::FAILURE;


    }


}

#include "behaviortree_cpp_v3/bt_factory.h"

BT_REGISTER_NODES(factory)
{
  factory.registerSimpleCondition(
    "baseFailed",
    std::bind(&sentry_behavior_tree::baseFailed, std::placeholders::_1));
}



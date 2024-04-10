#include "sentry_behavior_tree/plugins/condition/counter_outpost_condition.hpp"
#include "math.h"
namespace sentry_behavior_tree
{

    BT::NodeStatus counterOutpost(BT::TreeNode & tree_node){

        auto mode = tree_node.config().blackboard->get<uint8_t>("mode");

        return mode == 0 ? BT::NodeStatus::SUCCESS : BT::NodeStatus::FAILURE;

    }


}

#include "behaviortree_cpp_v3/bt_factory.h"

BT_REGISTER_NODES(factory)
{
  factory.registerSimpleCondition(
    "counterOutpost",
    std::bind(&sentry_behavior_tree::counterOutpost, std::placeholders::_1));
}



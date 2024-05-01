#include "sentry_behavior_tree/plugins/condition/counter_attack_condition.hpp"
#include "math.h"
namespace sentry_behavior_tree
{

    BT::NodeStatus counterAttack(BT::TreeNode & tree_node){

        auto counter_attack = tree_node.config().blackboard->get<bool>("counter_attack");

        return counter_attack ? BT::NodeStatus::SUCCESS : BT::NodeStatus::FAILURE;

    }


}

#include "behaviortree_cpp_v3/bt_factory.h"

BT_REGISTER_NODES(factory)
{
  factory.registerSimpleCondition(
    "counterAttack",
    std::bind(&sentry_behavior_tree::counterAttack, std::placeholders::_1));
}



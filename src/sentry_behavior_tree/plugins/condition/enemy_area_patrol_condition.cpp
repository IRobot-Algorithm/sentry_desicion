#include "sentry_behavior_tree/plugins/condition/enemy_area_patrol_condition.hpp"
#include "math.h"
namespace sentry_behavior_tree
{

    BT::NodeStatus enemyAreaPatrol(BT::TreeNode & tree_node){

        auto mode = tree_node.config().blackboard->get<uint8_t>("mode");
        auto enemy_area = tree_node.config().blackboard->get<bool>("enemy_area");

        return (mode == 1 && enemy_area) ? BT::NodeStatus::SUCCESS : BT::NodeStatus::FAILURE;

    }


}

#include "behaviortree_cpp_v3/bt_factory.h"

BT_REGISTER_NODES(factory)
{
  factory.registerSimpleCondition(
    "enemyAreaPatrol",
    std::bind(&sentry_behavior_tree::enemyAreaPatrol, std::placeholders::_1));
}



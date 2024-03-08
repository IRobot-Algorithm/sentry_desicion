#include "sentry_behavior_tree/plugins/condition/game_start_condition.hpp"
#include "math.h"
namespace sentry_behavior_tree
{


    BT::NodeStatus gameStart(BT::TreeNode & tree_node){

        auto game_start = tree_node.config().blackboard->get<bool>("game_start");

        return game_start ? BT::NodeStatus::SUCCESS : BT::NodeStatus::FAILURE;

    }


}

#include "behaviortree_cpp_v3/bt_factory.h"

BT_REGISTER_NODES(factory)
{
  factory.registerSimpleCondition(
    "gameStart",
    std::bind(&sentry_behavior_tree::gameStart, std::placeholders::_1));
}



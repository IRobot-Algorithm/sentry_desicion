#include "sentry_behavior_tree/plugins/condition/outpost_stalled_condition.hpp"
#include "math.h"
namespace sentry_behavior_tree
{


    BT::NodeStatus outpostStalled(BT::TreeNode & tree_node){

        auto game_start = tree_node.config().blackboard->get<bool>("game_start");
        auto gameover_time = tree_node.config().blackboard->get<u_int16_t>("gameover_time");
        bool outpost_stalled = game_start && (gameover_time < 240);

        return outpost_stalled ? BT::NodeStatus::SUCCESS : BT::NodeStatus::FAILURE;

    }


}

#include "behaviortree_cpp_v3/bt_factory.h"

BT_REGISTER_NODES(factory)
{
  factory.registerSimpleCondition(
    "outpostStalled",
    std::bind(&sentry_behavior_tree::outpostStalled, std::placeholders::_1));
}



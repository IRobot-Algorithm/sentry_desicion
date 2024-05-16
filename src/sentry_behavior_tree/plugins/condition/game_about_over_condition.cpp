#include "sentry_behavior_tree/plugins/condition/game_about_over_condition.hpp"
#include "math.h"
namespace sentry_behavior_tree
{


    BT::NodeStatus gameAboutOver(BT::TreeNode & tree_node){

        auto game_start = tree_node.config().blackboard->get<bool>("game_start");
        auto gameover_time = tree_node.config().blackboard->get<u_int16_t>("gameover_time");
        bool game_about_over = game_start && (gameover_time < 20);

        return game_about_over ? BT::NodeStatus::SUCCESS : BT::NodeStatus::FAILURE;

    }


}

#include "behaviortree_cpp_v3/bt_factory.h"

BT_REGISTER_NODES(factory)
{
  factory.registerSimpleCondition(
    "gameAboutOver",
    std::bind(&sentry_behavior_tree::gameAboutOver, std::placeholders::_1));
}



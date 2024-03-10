#include "sentry_behavior_tree/plugins/condition/game_about_over_condition.hpp"
#include "math.h"
namespace sentry_behavior_tree
{


    BT::NodeStatus gameAboutOver(BT::TreeNode & tree_node){

        auto gameover_time = tree_node.config().blackboard->get<u_int16_t>("gameover_time");

        return gameover_time < 30 ? BT::NodeStatus::SUCCESS : BT::NodeStatus::FAILURE;

    }


}

#include "behaviortree_cpp_v3/bt_factory.h"

BT_REGISTER_NODES(factory)
{
  factory.registerSimpleCondition(
    "gameAboutOver",
    std::bind(&sentry_behavior_tree::gameAboutOver, std::placeholders::_1));
}



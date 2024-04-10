#include "sentry_behavior_tree/plugins/condition/enemy_outpost_survives_condition.hpp"
#include "math.h"
namespace sentry_behavior_tree
{


    BT::NodeStatus enemyOutpostSurvives(BT::TreeNode & tree_node){

        auto outpost_hp = tree_node.config().blackboard->get<u_int16_t>("outpost_hp");

        return outpost_hp > 0 ? BT::NodeStatus::SUCCESS : BT::NodeStatus::FAILURE;

    }


}

#include "behaviortree_cpp_v3/bt_factory.h"

BT_REGISTER_NODES(factory)
{
  factory.registerSimpleCondition(
    "enemyOutpostSurvives",
    std::bind(&sentry_behavior_tree::enemyOutpostSurvives, std::placeholders::_1));
}



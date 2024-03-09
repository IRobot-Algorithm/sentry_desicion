#include "sentry_behavior_tree/plugins/condition/outpost_survives_condition.hpp"
#include "math.h"
namespace sentry_behavior_tree
{


    BT::NodeStatus outpostSurvives(BT::TreeNode & tree_node){

        auto our_outpost_hp = tree_node.config().blackboard->get<u_int16_t>("our_outpost_hp_");

        return our_outpost_hp > 0 ? BT::NodeStatus::SUCCESS : BT::NodeStatus::FAILURE;

    }


}

#include "behaviortree_cpp_v3/bt_factory.h"

BT_REGISTER_NODES(factory)
{
  factory.registerSimpleCondition(
    "outpostSurvives",
    std::bind(&sentry_behavior_tree::outpostSurvives, std::placeholders::_1));
}



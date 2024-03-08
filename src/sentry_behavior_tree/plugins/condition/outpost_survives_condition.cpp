#include "sentry_behavior_tree/plugins/condition/outpost_survives_condition.hpp"
#include "math.h"
namespace sentry_behavior_tree
{


    BT::NodeStatus outpostSurvives(BT::TreeNode & tree_node){

        auto outpost_survives = tree_node.config().blackboard->get<bool>("outpost_survives");

        return outpost_survives ? BT::NodeStatus::SUCCESS : BT::NodeStatus::FAILURE;

    }


}

#include "behaviortree_cpp_v3/bt_factory.h"

BT_REGISTER_NODES(factory)
{
  factory.registerSimpleCondition(
    "outpostSurvives",
    std::bind(&sentry_behavior_tree::outpostSurvives, std::placeholders::_1));
}



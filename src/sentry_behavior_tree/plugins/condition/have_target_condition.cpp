#include "sentry_behavior_tree/plugins/condition/have_target_condition.hpp"
#include "math.h"
namespace sentry_behavior_tree
{


    BT::NodeStatus haveTarget(BT::TreeNode & tree_node){

        auto have_target = tree_node.config().blackboard->get<bool>("have_target");

        if (have_target >= 1)
          return BT::NodeStatus::SUCCESS;
        else
          return BT::NodeStatus::FAILURE;

    }


}

#include "behaviortree_cpp_v3/bt_factory.h"

BT_REGISTER_NODES(factory)
{
  factory.registerSimpleCondition(
    "haveTarget",
    std::bind(&sentry_behavior_tree::haveTarget, std::placeholders::_1));
}



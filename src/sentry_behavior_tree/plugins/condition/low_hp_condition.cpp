#include "sentry_behavior_tree/plugins/condition/low_hp_condition.hpp"
#include "math.h"
namespace sentry_behavior_tree
{


    BT::NodeStatus lowHp(BT::TreeNode & tree_node){

        auto base_unfolds = tree_node.config().blackboard->get<int>("robot_hp") <= 150;

        return base_unfolds ? BT::NodeStatus::SUCCESS : BT::NodeStatus::FAILURE;


    }


}

#include "behaviortree_cpp_v3/bt_factory.h"

BT_REGISTER_NODES(factory)
{
  factory.registerSimpleCondition(
    "lowHp",
    std::bind(&sentry_behavior_tree::lowHp, std::placeholders::_1));
}



#include "sentry_behavior_tree/plugins/condition/target_can_follow_condition.hpp"
#include "math.h"
#include "geometry_msgs/msg/point_stamped.hpp"
namespace sentry_behavior_tree
{


    BT::NodeStatus targetCanFollow(BT::TreeNode & tree_node){

        auto target_pos = tree_node.config().blackboard->get<geometry_msgs::msg::PointStamped>("target_pos");

        return target_pos.point.z < 0.1 ? BT::NodeStatus::SUCCESS : BT::NodeStatus::FAILURE;

    }


}

#include "behaviortree_cpp_v3/bt_factory.h"

BT_REGISTER_NODES(factory)
{
  factory.registerSimpleCondition(
    "targetCanFollow",
    std::bind(&sentry_behavior_tree::targetCanFollow, std::placeholders::_1));
}



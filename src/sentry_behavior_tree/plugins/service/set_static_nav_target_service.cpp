#include <string>
#include "sentry_behavior_tree/plugins/service/set_static_nav_target_service.hpp"

namespace sentry_behavior_tree{

    SetStaticNavTargetService::SetStaticNavTargetService(const std::string & service_node_name,
        const BT::NodeConfiguration & conf)
        : nav2_behavior_tree::BtServiceNode<sentry_srvs::srv::NavTarget>(service_node_name, conf){}

    void SetStaticNavTargetService::on_tick()
    {
        u_int8_t have_target;
        bool gimbal;
        geometry_msgs::msg::PointStamped target_pos;
        getInput("have_target", have_target);;
        getInput("gimbal", gimbal);
        getInput("target_pos", target_pos);
        request_->is_lost = have_target == 3;
        request_->gimbal = gimbal;
        request_->is_dynamic = false;
        request_->distance = 3.0;
        request_->restricted_area = 0;
        request_->pose.header.stamp = target_pos.header.stamp;
        request_->pose.pose.position.x = target_pos.point.x;
        request_->pose.pose.position.y = target_pos.point.y;
        request_->pose.pose.position.z = target_pos.point.z;

        // RCLCPP_INFO(node_->get_logger(),"set_static_nav_target_service on_tick()... ");
    }

    BT::NodeStatus SetStaticNavTargetService::check_future(
    std::shared_future<sentry_srvs::srv::NavTarget::Response::SharedPtr> future_result)
    {
        rclcpp::FutureReturnCode rc;
        rc = rclcpp::spin_until_future_complete(
        node_,
        future_result, server_timeout_);
        if (rc == rclcpp::FutureReturnCode::SUCCESS)
        {
            auto result = future_result.get();
            if (result->success)
                return BT::NodeStatus::SUCCESS;
            else
                return BT::NodeStatus::FAILURE;
        }
        // TODO : 无法跟随
        else if (rc == rclcpp::FutureReturnCode::TIMEOUT)
        {
            RCLCPP_WARN(
                node_->get_logger(),
                "Node timed out while executing service call to %s.", service_name_.c_str());
            on_wait_for_result();
        }
        return BT::NodeStatus::FAILURE;
    }

    BT::PortsList SetStaticNavTargetService::providedPorts()
    {
        return providedBasicPorts(
        {
            BT::InputPort<u_int8_t>("have_target"),
            BT::InputPort<bool>("gimbal"),
            BT::InputPort<geometry_msgs::msg::PointStamped>("target_pos"),
        });
    }

}

#include "behaviortree_cpp_v3/bt_factory.h"
BT_REGISTER_NODES(factory)
{
  factory.registerNodeType<sentry_behavior_tree::SetStaticNavTargetService>(
    "SetStaticNavTarget");
    //注意这里""内没有Service 
}
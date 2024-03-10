#include <string>
#include "sentry_behavior_tree/plugins/service/set_right_target_service.hpp"

namespace sentry_behavior_tree{

    SetRightTargetService::SetRightTargetService(const std::string & service_node_name,
        const BT::NodeConfiguration & conf)
        : nav2_behavior_tree::BtServiceNode<sentry_interfaces::srv::AimTarget>(service_node_name, conf){}

    void SetRightTargetService::on_tick()
    {
        std::vector<u_int8_t> list;
        getInput("list", list);
        request_->list = list;

        RCLCPP_INFO(node_->get_logger(),"set_right_target_service on_tick()... ");
    }

    BT::NodeStatus SetRightTargetService::check_future(
    std::shared_future<sentry_interfaces::srv::AimTarget::Response::SharedPtr> future_result)
    {
        rclcpp::FutureReturnCode rc;
        rc = rclcpp::spin_until_future_complete(
        node_,
        future_result, server_timeout_);
        if (rc == rclcpp::FutureReturnCode::SUCCESS)
            return BT::NodeStatus::SUCCESS;
        else if (rc == rclcpp::FutureReturnCode::TIMEOUT)
        {
            RCLCPP_WARN(
                node_->get_logger(),
                "Node timed out while executing service call to %s.", service_name_.c_str());
            on_wait_for_result();
        }
        return BT::NodeStatus::FAILURE;
    }

    BT::PortsList SetRightTargetService::providedPorts()
    {
        return providedBasicPorts(
        {
            BT::InputPort<std::vector<u_int8_t>>("list"),
        });
    }

}

#include "behaviortree_cpp_v3/bt_factory.h"
BT_REGISTER_NODES(factory)
{
  factory.registerNodeType<sentry_behavior_tree::SetRightTargetService>(
    "SetRightTarget");
    //注意这里""内没有Service 
}
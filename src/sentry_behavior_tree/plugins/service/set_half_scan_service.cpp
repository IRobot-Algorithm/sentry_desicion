#include <string>
#include "sentry_behavior_tree/plugins/service/set_half_scan_service.hpp"

namespace sentry_behavior_tree{

    SetHalfScanService::SetHalfScanService(const std::string & service_node_name,
        const BT::NodeConfiguration & conf)
        : nav2_behavior_tree::BtServiceNode<sentry_interfaces::srv::HalfScan>(service_node_name, conf){}

    void SetHalfScanService::on_tick()
    {
        request_->half_scan = true;

        // RCLCPP_INFO(node_->get_logger(),"set_half_scan_service on_tick()... ");
    }

    BT::NodeStatus SetHalfScanService::check_future(
    std::shared_future<sentry_interfaces::srv::HalfScan::Response::SharedPtr> future_result)
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

}

#include "behaviortree_cpp_v3/bt_factory.h"
BT_REGISTER_NODES(factory)
{
  factory.registerNodeType<sentry_behavior_tree::SetHalfScanService>(
    "SetHalfScan");
    //注意这里""内没有Service 
}
#include <string>
#include "sentry_behavior_tree/plugins/service/rmuc_is_save_service.hpp"

namespace sentry_behavior_tree{

    RmucIsSaveService::RmucIsSaveService(const std::string & service_node_name,
        const BT::NodeConfiguration & conf)
        : nav2_behavior_tree::BtServiceNode<sentry_srvs::srv::NavGoal>(service_node_name, conf){}

    void RmucIsSaveService::on_tick()
    {

        request_->pose.pose.position.x = -100.0;
        request_->pose.pose.position.y = 0.0;

        // RCLCPP_INFO(node_->get_logger(),"rmuc_is_save_service on_tick()... ");
    }

    BT::NodeStatus RmucIsSaveService::check_future(
    std::shared_future<sentry_srvs::srv::NavGoal::Response::SharedPtr> future_result)
    {
        rclcpp::FutureReturnCode rc;
        rc = rclcpp::spin_until_future_complete(
        node_,
        future_result, server_timeout_);
        if (rc == rclcpp::FutureReturnCode::SUCCESS)
        {
            auto result = future_result.get();
            if (result->is_arrive)
                return BT::NodeStatus::SUCCESS;
            else
                return BT::NodeStatus::FAILURE;
        }
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
  factory.registerNodeType<sentry_behavior_tree::RmucIsSaveService>(
    "RmucIsSave");
    //注意这里""内没有Service 
}
#include <string>
#include "sentry_behavior_tree/plugins/service/buy_bullets_service.hpp"

namespace sentry_behavior_tree{

    BuyBulletsService::BuyBulletsService(const std::string & service_node_name,
        const BT::NodeConfiguration & conf)
        : nav2_behavior_tree::BtServiceNode<sentry_srvs::srv::BuyBullets>(service_node_name, conf){}

    void BuyBulletsService::on_tick()
    {
        u_int16_t buy_bullets;
        getInput("buy_bullets", buy_bullets);
        request_->bullets = buy_bullets;

        // RCLCPP_INFO(node_->get_logger(),"buy_bullets_service on_tick()... ");
    }

    BT::NodeStatus BuyBulletsService::check_future(
    std::shared_future<sentry_srvs::srv::BuyBullets::Response::SharedPtr> future_result)
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

    BT::PortsList BuyBulletsService::providedPorts()
    {
        return providedBasicPorts(
        {
            BT::InputPort<u_int16_t>("buy_bullets"),
        });
    }

}

#include "behaviortree_cpp_v3/bt_factory.h"
BT_REGISTER_NODES(factory)
{
  factory.registerNodeType<sentry_behavior_tree::BuyBulletsService>(
    "BuyBullets");
    //注意这里""内没有Service 
}
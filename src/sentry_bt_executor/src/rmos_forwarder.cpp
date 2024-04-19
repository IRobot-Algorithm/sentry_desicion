#include "sentry_bt_executor/rmos_forwarder.hpp"
#include <chrono> 

namespace sentry_bt_executor
{


RmosForwarder::RmosForwarder(const rclcpp::NodeOptions &options) : Node("rmos_forwarder", options) 
{
  RCLCPP_INFO(this->get_logger(), "RmosForwarder Start");

  list_service_ = this->create_service<sentry_interfaces::srv::AimTarget>(
      "aim_target",
      std::bind(&RmosForwarder::listServiceCallback, this,
                std::placeholders::_1, std::placeholders::_2));

  client_l_ = this->create_client<sentry_interfaces::srv::AimTarget>("/rmos_processer_l/AimTarget_l");
  client_r_ = this->create_client<sentry_interfaces::srv::AimTarget>("/rmos_processer_r/AimTarget_r");

  timer_callback_group_ = this->create_callback_group(
    rclcpp::CallbackGroupType::MutuallyExclusive);

  timer_ = this->create_wall_timer(std::chrono::seconds(1), 
    std::bind(&RmosForwarder::forwardList, this), timer_callback_group_);
}

void RmosForwarder::listServiceCallback(
    const std::shared_ptr<sentry_interfaces::srv::AimTarget::Request> request,
    std::shared_ptr<sentry_interfaces::srv::AimTarget::Response> response) 
{
  received_list_ = request->list;
  last_receive_time_ = rclcpp::Clock().now();
  response->success = true;
}

void RmosForwarder::forwardList() 
{
  if (!client_l_->service_is_ready() || !client_r_->service_is_ready()) 
  {
    RCLCPP_WARN(this->get_logger(), "One of the services is not available");
    return;
  }
  if (rclcpp::Clock().now().seconds() - last_receive_time_.seconds() > 5.0)
  {
    RCLCPP_WARN(this->get_logger(), "Last receive too long");
    return;
  } 

  auto request = std::make_shared<sentry_interfaces::srv::AimTarget::Request>();
  request->list = received_list_;

  auto future_l = client_l_->async_send_request(request);
  auto future_r = client_r_->async_send_request(request);
}


} // namespace sentry_bt_executor

#include <rclcpp_components/register_node_macro.hpp>

// Register the component with class_loader.
// This acts as a sort of entry point, allowing the component to be discoverable when its library
// is being loaded into a running process.
RCLCPP_COMPONENTS_REGISTER_NODE(sentry_bt_executor::RmosForwarder)
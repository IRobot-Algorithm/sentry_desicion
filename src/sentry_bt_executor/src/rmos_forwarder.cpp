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

  client_ = this->create_client<sentry_interfaces::srv::AimTarget>("/rmos_processer/AimTarget");

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
  if (rclcpp::Clock().now().seconds() - last_receive_time_.seconds() > 1.0)
  {
    RCLCPP_WARN(this->get_logger(), "Last receive too long");
    return;
  } 

  auto request = std::make_shared<sentry_interfaces::srv::AimTarget::Request>();
  request->list = received_list_;

  if (client_->service_is_ready()) 
  {
    auto future = client_->async_send_request(request);
  }
  else
  {
    RCLCPP_WARN(this->get_logger(), "Services is not available");
  }


}


} // namespace sentry_bt_executor

#include <rclcpp_components/register_node_macro.hpp>

// Register the component with class_loader.
// This acts as a sort of entry point, allowing the component to be discoverable when its library
// is being loaded into a running process.
RCLCPP_COMPONENTS_REGISTER_NODE(sentry_bt_executor::RmosForwarder)
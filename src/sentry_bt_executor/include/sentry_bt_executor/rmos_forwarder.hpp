#ifndef SENTRY_BT_EXECUTOR__RMOS_FORWARDER_HPP_
#define SENTRY_BT_EXECUTOR__RMOS_FORWARDER_HPP_

#include "rclcpp/rclcpp.hpp"
#include "sentry_interfaces/srv/aim_target.hpp"
#include "sentry_interfaces/srv/half_scan.hpp"

namespace sentry_bt_executor
{


class RmosForwarder : public rclcpp::Node 
{
public:
  RmosForwarder();

  RmosForwarder(const rclcpp::NodeOptions &options);

private:
  void listServiceCallback(
      const std::shared_ptr<sentry_interfaces::srv::AimTarget::Request> request,
      std::shared_ptr<sentry_interfaces::srv::AimTarget::Response> response);
  
  void HalfScanServiceCallback(
      const std::shared_ptr<sentry_interfaces::srv::HalfScan::Request> request,
      std::shared_ptr<sentry_interfaces::srv::HalfScan::Response> response);

  void forwardList();
 
  rclcpp::CallbackGroup::SharedPtr timer_callback_group_;
  rclcpp::Service<sentry_interfaces::srv::AimTarget>::SharedPtr list_service_;
  rclcpp::Service<sentry_interfaces::srv::HalfScan>::SharedPtr half_scan_service_;
  rclcpp::Client<sentry_interfaces::srv::AimTarget>::SharedPtr client_l_;
  rclcpp::Client<sentry_interfaces::srv::AimTarget>::SharedPtr client_r_;
  std::vector<uint8_t> received_list_;
  rclcpp::TimerBase::SharedPtr timer_;
  rclcpp::Time last_receive_time_;
  rclcpp::Time half_scan_time_;
};


} // namespace sentry_bt_executor

#endif

#ifndef DECISION_COMM__COMM_NODE_HPP_
#define DECISION_COMM__COMM_NODE_HPP_

#include <memory>
#include <chrono>
#include <iostream>
#include <functional>
#include <thread>


#include <rclcpp/rclcpp.hpp>
#include <tf2_ros/transform_broadcaster.h>
#include <geometry_msgs/msg/quaternion_stamped.hpp>
#include <geometry_msgs/msg/twist.hpp>
#include <geometry_msgs/msg/twist_stamped.hpp>
#include <geometry_msgs/msg/point_stamped.hpp>
#include <geometry_msgs/msg/pose.hpp>
#include <geometry_msgs/msg/pose_stamped.hpp>
#include <std_msgs/msg/bool.hpp>
#include <std_msgs/msg/int16.hpp>
#include <std_msgs/msg/float32.hpp>

#include "sentry_comm/can.hpp"
#include "rclcpp_action/rclcpp_action.hpp"
#include "sentry_interfaces/msg/referee_information.hpp"

using namespace std::chrono;

namespace sentry_comm
{


class CommNode : public rclcpp::Node {

  public:
    CommNode(const std::string &node_name, const rclcpp::NodeOptions &options) : Node(node_name, options)
    {
      RCLCPP_INFO(this->get_logger(), "Start Communicate Node: %s", node_name.c_str());

      this->referee_information_pub_ = this->create_publisher<sentry_interfaces::msg::RefereeInformation>("/referee_information", rclcpp::SensorDataQoS());
    }

  protected:
    sentry_interfaces::msg::RefereeInformation referee_information_;
    rclcpp::Publisher<sentry_interfaces::msg::RefereeInformation>::SharedPtr referee_information_pub_;

    std::thread recevie_thread_;

  };

class CanCommNode : public CommNode {

  public:
    CanCommNode(const rclcpp::NodeOptions & options);
    ~CanCommNode();     

  protected:
    /**
    *  @brief  发送裁判系统信息的回调函数
    */
    void sendRefereeInformationCallBack();

    /**
    *  @brief  读取can数据的线程函数
    */
    void recevieCallBack();

    transporter::Can can_;   // can通信接口

    /* Send Timer */
    rclcpp::TimerBase::SharedPtr referee_information_timer_;

    /* Callback Group */
    rclcpp::CallbackGroup::SharedPtr send_infomation_callback_group_;

};


} // namespace sentry_comm

#endif
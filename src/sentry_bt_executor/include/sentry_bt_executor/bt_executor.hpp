#ifndef SENTRY_BT_EXECUTOR__BT_EXECUTOR_HPP_
#define SENTRY_BT_EXECUTOR__BT_EXECUTOR_HPP_

#include <memory>
#include <string>
#include <vector>

#include "nav2_behavior_tree/behavior_tree_engine.hpp"
#include "nav2_util/lifecycle_node.hpp"
#include "nav2_util/simple_action_server.hpp"
#include "rclcpp_action/rclcpp_action.hpp"

#include "geometry_msgs/msg/pose_stamped.hpp"
#include "geometry_msgs/msg/point_stamped.hpp"

#include "sentry_interfaces/msg/follow_target.hpp"
#include "sentry_msgs/msg/referee_information.hpp"


namespace sentry_bt_executor{


class BtExecutor : public rclcpp::Node{

public:
    BtExecutor();

    BtExecutor(const rclcpp::NodeOptions &options);

    ~BtExecutor();

protected:
    bool loadBehaviorTree(const std::string & bt_xml_filename);

    void executeBehaviorTree();

    BT::Tree tree_;

    // The blackboard shared by all of the nodes in the tree
    BT::Blackboard::Ptr blackboard_;

    // The XML fiñe that cointains the Behavior Tree to create
    //std::string current_bt_xml_filename_;
    std::string default_bt_xml_filename_;

    const char* xml_text_;

    // The wrapper class for the BT functionality
    std::unique_ptr<nav2_behavior_tree::BehaviorTreeEngine> bt_;

    // Libraries to pull plugins (BT Nodes) from
    std::vector<std::string> plugin_lib_names_;

    // A regular, non-spinning ROS node that we can use for calls to the action client
    rclcpp::Node::SharedPtr client_node_;

    // Metrics for feedback
    rclcpp::Time start_time_;

private:
    void judgeTarget();

    void refereeInformationCallback(const sentry_msgs::msg::RefereeInformation::SharedPtr referee_information);

    void rightRmosCallback(const sentry_interfaces::msg::FollowTarget::SharedPtr follow_target);
    void leftRmosCallback(const sentry_interfaces::msg::FollowTarget::SharedPtr follow_target);

private:
    rclcpp::Subscription<sentry_msgs::msg::RefereeInformation>::SharedPtr referee_information_sub_;
    rclcpp::Subscription<sentry_interfaces::msg::FollowTarget>::SharedPtr right_rmos_sub_;
    rclcpp::Subscription<sentry_interfaces::msg::FollowTarget>::SharedPtr left_rmos_sub_;

    rclcpp::CallbackGroup::SharedPtr referee_information_sub_callback_group_;
    rclcpp::CallbackGroup::SharedPtr rmos_sub_callback_group_;
    rclcpp::CallbackGroup::SharedPtr execute_timer_callback_group_;

    rclcpp::TimerBase::SharedPtr execute_timer_;

    rclcpp::Time time_ = rclcpp::Clock().now();
    double leave_time_ = 0; // 离开巡逻区的时间

    /* 我方机器人信息 */
    u_int16_t robot_hp_ = 0;
    u_int16_t max_hp_ = 0;
    u_int16_t bullets_ = 400;
    u_int16_t our_outpost_hp_ = 0;
    u_int16_t our_base_hp_ = 1000;
    u_int8_t base_shield_ = 100;
    u_int16_t gold_coins_ = 0; 

    /* 比赛状态信息 */
    bool game_start_ = false;
    u_int16_t gameover_time_;

    /* 敌方机器人血量及自瞄状态 */
    std::vector<u_int16_t> enemy_hp_;
    u_int8_t have_target_ = 0; // 0 for no target, 1 for tracking, 2 for lost
    u_int8_t left_target_ = 0; // 0 for no target, 1 for tracking, 2 for lost
    u_int8_t right_target_ = 0; // 0 for no target, 1 for tracking, 2 for lost
    geometry_msgs::msg::PointStamped left_target_pos_;
    geometry_msgs::msg::PointStamped right_target_pos_;
    bool gimbal_; // 0 for right, 1 for left
    geometry_msgs::msg::PointStamped target_pos_;

    bool air_force_ = false; // 敌方空中机器人信息
    bool force_back_ = false; // 强制回家

    std::vector<u_int8_t> list_; // 自瞄目标

    bool in_supply_ = false;
    double supply_time_ = 0.0;

};


} // namespace sentry_bt_executor



#endif
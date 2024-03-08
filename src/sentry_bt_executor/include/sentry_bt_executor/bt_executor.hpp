#ifndef SENTRY_BT_EXECUTOR__BT_EXECUTOR_HPP_
#define SENTRY_BT_EXECUTOR__BT_EXECUTOR_HPP_

#include <memory>
#include <string>
#include <vector>
#include <time.h>

#include "nav2_behavior_tree/behavior_tree_engine.hpp"
#include "nav2_util/lifecycle_node.hpp"
#include "nav2_util/simple_action_server.hpp"
#include "rclcpp_action/rclcpp_action.hpp"

#include "sentry_interfaces/msg/referee_information.hpp"
#include "rmos_interfaces/msg/armor.hpp"
#include "rmos_interfaces/msg/armors.hpp"

#include "std_msgs/msg/bool.hpp"
#include "std_msgs/msg/int16.hpp"
#include "geometry_msgs/msg/pose_stamped.hpp"
#include "nav_msgs/msg/odometry.hpp"

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

    void odometryCallback(const nav_msgs::msg::Odometry::SharedPtr odometry);

    void refereeInformationCallback(const sentry_interfaces::msg::RefereeInformation::SharedPtr referee_information);

    void armorsCallback(const rmos_interfaces::msg::Armors::SharedPtr armors);

    void judgeArea();

private:
    rclcpp::Subscription<nav_msgs::msg::Odometry>::SharedPtr odometry_sub_;
    rclcpp::Subscription<sentry_interfaces::msg::RefereeInformation>::SharedPtr referee_information_sub_;
    rclcpp::Subscription<rmos_interfaces::msg::Armors>::SharedPtr armors_sub_;

    rclcpp::CallbackGroup::SharedPtr odometry_sub_callback_group_;
    rclcpp::CallbackGroup::SharedPtr referee_information_sub_callback_group_;
    rclcpp::CallbackGroup::SharedPtr armors_sub_callback_group_;
    rclcpp::CallbackGroup::SharedPtr judge_area_timer_callback_group_;
    rclcpp::CallbackGroup::SharedPtr execute_timer_callback_group_;

    rclcpp::TimerBase::SharedPtr judge_area_timer_;
    rclcpp::TimerBase::SharedPtr execute_timer_;

    /* 哨兵定位信息 */
    nav_msgs::msg::Odometry sentry_odom_;
    bool in_patrol_area_ = false;
    bool in_supply_area_ = false;
    bool in_save_space_ = false;
    double leave_time_ = 0; // 离开巡逻区的时间

    /* 我方机器人信息 */
    int robot_hp_ = 0;
    int bullets_ = 400;
    int our_outpost_hp_ = 0;
    bool outpost_survives_ = true;
    int our_base_hp_ = 1000;
    bool base_unfolds_ = false;
    int gold_coins_ = 0; 

    /* 比赛状态信息 */
    bool game_start_ = false;
    double gameover_time_;

    /* 敌方机器人血量及自瞄状态 */
    std::vector<int> enemy_hp_;
    std::vector<bool> find_armors_;

    bool air_force_ = false; // 敌方空中机器人信息

};


} // namespace sentry_bt_executor



#endif
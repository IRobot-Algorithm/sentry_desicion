#ifndef SENTRY_BT_EXECUTOR__BT_EXECUTOR_HPP_
#define SENTRY_BT_EXECUTOR__BT_EXECUTOR_HPP_

#include <memory>
#include <string>
#include <vector>

#include "behaviortree_cpp_v3/loggers/bt_cout_logger.h"
#include "behaviortree_cpp_v3/loggers/bt_file_logger.h"
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
    /*
	* @brief 与导航跟随自瞄目标相关决策
	*/
    void judgeTarget();

    /*
	* @brief 与购买弹丸相关决策
    * @details 计算购买的弹丸量是实时进行的 但只在检测到rfid才更新串口协议
	*/
    void judgeBullets();

    void refereeInformationCallback(const sentry_msgs::msg::RefereeInformation::SharedPtr referee_information);

    void rightRmosCallback(const sentry_interfaces::msg::FollowTarget::SharedPtr follow_target);
    void leftRmosCallback(const sentry_interfaces::msg::FollowTarget::SharedPtr follow_target);

	/*
	* @brief uint32 pos位赋值1
	*/
    inline void setBit(uint32_t& data, int pos);

	/*
	* @brief uint32 pos位赋值0
	*/
    inline void clearBit(uint32_t& data, int pos);

	/*
	* @brief 获得 uint32 pos位
	*/
    inline bool getBit(const uint32_t& data, int pos);

	/*
	* @brief uint32 start到end位赋值为value
	*/
    void setBitsRange(uint32_t &data, int start, int end, uint32_t value);

private:
    rclcpp::Subscription<sentry_msgs::msg::RefereeInformation>::SharedPtr referee_information_sub_;
    rclcpp::Subscription<sentry_interfaces::msg::FollowTarget>::SharedPtr right_rmos_sub_;
    rclcpp::Subscription<sentry_interfaces::msg::FollowTarget>::SharedPtr left_rmos_sub_;

    rclcpp::CallbackGroup::SharedPtr referee_information_sub_callback_group_;
    rclcpp::CallbackGroup::SharedPtr right_rmos_sub_callback_group_;
    rclcpp::CallbackGroup::SharedPtr left_rmos_sub_callback_group_;
    rclcpp::CallbackGroup::SharedPtr execute_timer_callback_group_;

    rclcpp::TimerBase::SharedPtr execute_timer_;

    /* 日志 */
    std::unique_ptr<BT::StdCoutLogger> cout_logger_;
    std::unique_ptr<BT::FileLogger> file_logger_;

    rclcpp::Time time_ = rclcpp::Clock().now();
    double leave_time_ = 0; // 离开巡逻区的时间

    /* 我方机器人信息 */
    u_int16_t robot_hp_ = 0;
    u_int16_t max_hp_ = 0;
    u_int16_t bullets_ = 400;
    u_int16_t bought_bullets_ = 0;
    u_int16_t our_outpost_hp_ = 0; // default
    u_int16_t our_base_hp_ = 3000;
    u_int8_t base_shield_ = 100;
    bool base_unfolds_ = false;
    u_int16_t gold_coins_ = 0; 

    /* 比赛状态信息 */
    bool game_start_ = false;
    u_int16_t gameover_time_;

    /* 敌方机器人血量及自瞄状态 */
    std::vector<u_int16_t> enemy_hp_;
    std::vector<double> enemy_died_time_;
    std::vector<bool> is_buyback_;
    u_int8_t have_target_ = 0; // 0 for no target, 1 for tracking, 2 for lost
    u_int8_t left_target_ = 0; // 0 for no target, 1 for tracking, 2 for lost
    u_int8_t right_target_ = 0; // 0 for no target, 1 for tracking, 2 for lost
    u_int8_t left_priority_;
    u_int8_t right_priority_;
    geometry_msgs::msg::PointStamped left_target_pos_;
    geometry_msgs::msg::PointStamped right_target_pos_;
    bool gimbal_; // 0 for right, 1 for left
    geometry_msgs::msg::PointStamped target_pos_;

    bool air_force_ = false; // 敌方空中机器人信息
    bool force_back_ = false; // 强制回家
    bool keep_patrol_ = false; // 保守巡逻
    uint8_t mode_ = 0; 

    std::vector<u_int8_t> low_hp_list_; // 自瞄目标
    std::vector<u_int8_t> invincibility_list_; // 无敌目标

    // 购买弹丸
    u_int16_t buy_bullets_ = 0;
    bool can_buy_bullets_ = false;
    
    bool have_buff_ = true;
    bool count_outpost_ = true;
    bool enemy_area_ = true;
    bool counter_attack_ = false;
    u_int16_t last_enemy_outpost_hp_ = 1500;
    u_int16_t last_bullets_ = 400;

    bool need_unlock_ = false;
    bool in_supply_ = false;
    bool in_patrol_ = false;
    double supply_time_ = 0.0;

};


} // namespace sentry_bt_executor



#endif
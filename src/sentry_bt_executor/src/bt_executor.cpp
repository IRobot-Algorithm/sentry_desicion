#include <fstream>
#include <memory>
#include <streambuf>
#include <string>
#include <utility>
#include <vector>
#include <set>
#include <exception>
#include <chrono>
#include <functional>

#include "nav2_behavior_tree/bt_conversions.hpp"
#include "sentry_bt_executor/bt_executor.hpp"

using namespace std::placeholders;
using namespace std::chrono;

namespace sentry_bt_executor
{


BtExecutor::BtExecutor(const rclcpp::NodeOptions &options)
    : rclcpp::Node("bt_executor", "", options),
      start_time_(0)
{

    RCLCPP_INFO(get_logger(), "Creating");

    // 检查是否缺少逗号
    const std::vector<std::string> plugin_libs = 
    {
        "go_back_bt_node",
        "rmuc_go_enemy_outpost_bt_node",
        "rmuc_go_supply_bt_node",
        "rmuc_avoid_bt_node",
        "rmul_defense_bt_node",
        "rmul_patrol_bt_node",
        "rmul_patrol_conserv_bt_node",
        "rmul_go_enemy_base_bt_node",
        "rmul_go_our_base_bt_node",
        "rmul_go_supply_bt_node",
        "set_nav_target_bt_node",
        "set_static_nav_target_bt_node",
        "set_left_target_bt_node",
        "set_right_target_bt_node",
        "air_force_condition_bt_node",
        "in_supply_condition_bt_node",
        "base_unfolds_condition_bt_node",
        "base_failed_condition_bt_node",
        "base_winned_condition_bt_node",
        "enemy_area_patrol_condition_bt_node",
        "center_patrol_condition_bt_node",
        "counter_outpost_condition_bt_node",
        "have_target_condition_bt_node", 
        "low_hp_condition_bt_node", 
        "low_bullets_condition_bt_node", 
        "leave_too_long_condition_bt_node", 
        "enough_hp_condition_bt_node", 
        "game_start_condition_bt_node", 
        "game_about_over_condition_bt_node", 
        "outpost_low_hp_condition_bt_node", 
        "enemy_outpost_survives_condition_bt_node", 
        "outpost_survives_condition_bt_node", 
        "target_can_follow_condition_bt_node", 
        "rmul_can_supply_condition_bt_node", 
    };

    // Declare this node's parameters
    declare_parameter("default_bt_xml_filename");
    declare_parameter("plugin_lib_names", plugin_libs);

    RCLCPP_INFO(get_logger(), "Configuring");

    client_node_ = std::make_shared<rclcpp::Node>("_", options);

    plugin_lib_names_ = get_parameter("plugin_lib_names").as_string_array();

    bt_ = std::make_unique<nav2_behavior_tree::BehaviorTreeEngine>(plugin_lib_names_);
    blackboard_ = BT::Blackboard::create();
    blackboard_->set<rclcpp::Node::SharedPtr>("node", client_node_); 
    // TODO:time_out修改到合适的值
    blackboard_->set<std::chrono::milliseconds>("server_timeout", std::chrono::milliseconds(1000)); 

    double now_time = rclcpp::Clock().now().seconds();    
    blackboard_->set<double>("now_time", now_time);

    /* 哨兵定位信息 */
    blackboard_->set<double>("leave_time", leave_time_);

    /* 比赛状态信息 */
    blackboard_->set<bool>("game_start", game_start_);
    blackboard_->set<u_int16_t>("gameover_time", gameover_time_);

    /* 我方机器人信息 */
    blackboard_->set<u_int16_t>("robot_hp", robot_hp_);
    blackboard_->set<u_int16_t>("bullets", bullets_);
    blackboard_->set<u_int16_t>("our_outpost_hp", our_outpost_hp_);
    blackboard_->set<u_int16_t>("our_base_hp", our_base_hp_);
    blackboard_->set<u_int8_t>("base_shield", base_shield_);
    blackboard_->set<bool>("base_unfolds", base_unfolds_);
    blackboard_->set<u_int16_t>("gold_coins", gold_coins_);

    // TODO: 更改初始血量
    enemy_hp_.resize(8, 0);
    
    /* 敌方机器人血量及自瞄状态 */
    blackboard_->set<u_int16_t>("base_hp", enemy_hp_[0]);
    blackboard_->set<u_int16_t>("hero_hp", enemy_hp_[1]);
    blackboard_->set<u_int16_t>("engineer_hp", enemy_hp_[2]);
    blackboard_->set<u_int16_t>("infantry3_hp", enemy_hp_[3]);
    blackboard_->set<u_int16_t>("infantry4_hp", enemy_hp_[4]);
    blackboard_->set<u_int16_t>("infantry5_hp", enemy_hp_[5]);
    blackboard_->set<u_int16_t>("sentry_hp", enemy_hp_[6]);
    blackboard_->set<u_int16_t>("outpost_hp", enemy_hp_[7]);
    blackboard_->set<u_int8_t>("have_target", have_target_);
    blackboard_->set<bool>("gimbal", gimbal_);
    blackboard_->set<geometry_msgs::msg::PointStamped>("target_pos", target_pos_);    
    blackboard_->set<std::vector<u_int8_t>>("low_hp_list", low_hp_list_); // 血量少的目标

    blackboard_->set<bool>("force_back", force_back_); // 强制回家
    blackboard_->set<bool>("keep_patrol", keep_patrol_); // 保守巡逻
    blackboard_->set<uint8_t>("mode", mode_);

    blackboard_->set<double>("loop_time", 0);

    blackboard_->set<bool>("in_supply", in_supply_);
    blackboard_->set<bool>("in_patrol", in_patrol_);

    /*
    // rmul
    blackboard_->set<double>("supply_time", supply_time_);
    */

    // 裁判系统没有的信息
    blackboard_->set<bool>("air_force", air_force_); // 敌方空中机器人信息

    get_parameter("default_bt_xml_filename", default_bt_xml_filename_);
    std::cout << "default_bt_xml_filename_: " << default_bt_xml_filename_ << std::endl;

    if (!loadBehaviorTree(default_bt_xml_filename_))
    {
        RCLCPP_ERROR(get_logger(), "Error loading XML file: %s", default_bt_xml_filename_.c_str());
        // return
    }

    /* create callback group */ 
    this->referee_information_sub_callback_group_ = this->create_callback_group(rclcpp::CallbackGroupType::Reentrant);
    this->rmos_sub_callback_group_ = this->create_callback_group(rclcpp::CallbackGroupType::Reentrant);
    this->execute_timer_callback_group_ = this->create_callback_group(rclcpp::CallbackGroupType::Reentrant);

    /* create subscription */
    auto referee_information_sub_options = rclcpp::SubscriptionOptions();
    referee_information_sub_options.callback_group = this->referee_information_sub_callback_group_;
    referee_information_sub_ = this->create_subscription<sentry_msgs::msg::RefereeInformation>("/referee_info", 1,
        std::bind(&BtExecutor::refereeInformationCallback, this, _1), referee_information_sub_options);

    auto rmos_sub_options = rclcpp::SubscriptionOptions();
    rmos_sub_options.callback_group = this->rmos_sub_callback_group_;
    right_rmos_sub_ = this->create_subscription<sentry_interfaces::msg::FollowTarget>("/follow_target_r", rclcpp::SensorDataQoS(),
        std::bind(&BtExecutor::rightRmosCallback, this, _1), rmos_sub_options);
    left_rmos_sub_ = this->create_subscription<sentry_interfaces::msg::FollowTarget>("/follow_target_l", rclcpp::SensorDataQoS(),
        std::bind(&BtExecutor::leftRmosCallback, this, _1), rmos_sub_options);


    RCLCPP_INFO(get_logger(), "Activating");

    // TODO：可重入？？
    //0.01s

    execute_timer_ = this->create_wall_timer(10ms, std::bind(&BtExecutor::executeBehaviorTree, this), execute_timer_callback_group_);
}

BtExecutor::~BtExecutor()
{
    RCLCPP_INFO(get_logger(), "Destroying");
}

bool BtExecutor::loadBehaviorTree(const std::string &bt_xml_filename)
{

    std::ifstream xml_file;
    xml_file.open(bt_xml_filename);

    if (!xml_file.good())
    {
        RCLCPP_ERROR(get_logger(), "Couldn't open input XML file: %s", bt_xml_filename.c_str());
        return false;
    }

    auto xml_string = std::string(
        std::istreambuf_iterator<char>(xml_file),
        std::istreambuf_iterator<char>());

    // Create the Behavior Tree from the XML input
    tree_ = bt_->createTreeFromText(xml_string, blackboard_);

    return true;
}

void BtExecutor::executeBehaviorTree()
{

    auto is_canceling = [this]()
    {
        // RCLCPP_INFO(get_logger(), "running void is_canceling()... Canceling... ");
        /*
        TODO:写cancle
        */
            return false;
    };

    auto on_loop = [&]()
    {
        double now_time = rclcpp::Clock().now().seconds();
        double loop_time = now_time - time_.seconds();
        time_ = rclcpp::Clock().now();
        
        blackboard_->set<double>("now_time", now_time);
        // RCLCPP_INFO(get_logger()," now_time : %lf", now_time);

        /*
        // rmul
        if (in_supply_ && max_hp_ - robot_hp_ >= 100)
            supply_time_ += loop_time;
        RCLCPP_INFO(get_logger(), "supply_time: %f", supply_time_);

        blackboard_->set<double>("supply_time", supply_time_);
        */

        if (in_patrol_)
            leave_time_ = rclcpp::Clock().now().seconds();
            
        blackboard_->set<double>("leave_time", leave_time_);

        /* 比赛状态信息 */
        blackboard_->set<bool>("game_start", game_start_);
        blackboard_->set<u_int16_t>("gameover_time", gameover_time_);

        /* 我方机器人信息 */
        blackboard_->set<u_int16_t>("robot_hp", robot_hp_);
        blackboard_->set<u_int16_t>("bullets", bullets_);
        blackboard_->set<u_int16_t>("our_outpost_hp", our_outpost_hp_);
        blackboard_->set<u_int16_t>("our_base_hp", our_base_hp_);
        blackboard_->set<u_int8_t>("base_shield", base_shield_);

        if (!game_start_)
        {
            base_unfolds_ = false;
        }
        else if (our_outpost_hp_ <= 0 && robot_hp_ <= 0)
        {
            base_unfolds_ = true;
        }
        blackboard_->set<bool>("base_unfolds", base_unfolds_);
        blackboard_->set<u_int16_t>("gold_coins", gold_coins_);

        /* 敌方机器人血量及自瞄状态 */
        blackboard_->set<u_int16_t>("base_hp", enemy_hp_[0]);
        blackboard_->set<u_int16_t>("hero_hp", enemy_hp_[1]);
        blackboard_->set<u_int16_t>("engineer_hp", enemy_hp_[2]);
        blackboard_->set<u_int16_t>("infantry3_hp", enemy_hp_[3]);
        blackboard_->set<u_int16_t>("infantry4_hp", enemy_hp_[4]);
        blackboard_->set<u_int16_t>("infantry5_hp", enemy_hp_[5]);
        blackboard_->set<u_int16_t>("sentry_hp", enemy_hp_[6]);
        blackboard_->set<u_int16_t>("outpost_hp", enemy_hp_[7]);

        blackboard_->set<std::vector<u_int8_t>>("low_hp_list", low_hp_list_); // 血量少的目标

        judgeTarget();

        blackboard_->set<u_int8_t>("have_target", have_target_);
        blackboard_->set<bool>("gimbal", gimbal_);
        blackboard_->set<geometry_msgs::msg::PointStamped>("target_pos", target_pos_);

        blackboard_->set<bool>("in_supply", in_supply_);
        blackboard_->set<bool>("in_patrol", in_patrol_);

        /*
        // rmul
        blackboard_->set<double>("supply_time", supply_time_);
        */

        blackboard_->set<bool>("force_back", force_back_); // 强制回家
        blackboard_->set<bool>("keep_patrol", keep_patrol_); // 保守巡逻
        blackboard_->set<uint8_t>("mode", mode_);

        // 裁判系统没有的信息
        blackboard_->set<bool>("air_force", air_force_); // 敌方空中机器人信息

    };

    std::chrono::milliseconds loopTimeout = std::chrono::milliseconds(200);
    rclcpp::WallRate looprate(loopTimeout);
    // looprate.sleep();

    nav2_behavior_tree::BtStatus rc = bt_->run(&tree_, on_loop, is_canceling);

    switch (rc)
    {
        case nav2_behavior_tree::BtStatus::SUCCEEDED:
            RCLCPP_INFO(get_logger(), "behavior tree succeeded");
            // action_server_->succeeded_current();
            break;

        case nav2_behavior_tree::BtStatus::FAILED:
            RCLCPP_ERROR(get_logger(), "behavior_tree failed");
            // action_server_->terminate_current();
            break;

        case nav2_behavior_tree::BtStatus::CANCELED:
            RCLCPP_INFO(get_logger(), "behavior_tree canceled");
            // action_server_->terminate_all();
            break;

        default:
            break;
    }

    bt_->haltAllActions(tree_.rootNode());
}

void BtExecutor::judgeTarget()
{

    if (left_target_ > 0  && right_target_ == 0) // 左有右无
    {
        have_target_ = left_target_;
        gimbal_ = 0;
        target_pos_ = left_target_pos_;
    }
    else if (left_target_ == 0 && right_target_ > 0) // 右有左无
    {
        have_target_ = right_target_;
        gimbal_ = 1;
        target_pos_ = right_target_pos_;
    }
    else if (left_target_ > 0 && right_target_ > 0) // 都有
    {
        if (right_priority_ > left_priority_) // 左边更优先
        {
            have_target_ = left_target_;
            gimbal_ = 0;
            target_pos_ = left_target_pos_;
        }
        else // 右边更优先
        {
            have_target_ = right_target_;
            gimbal_ = 1;
            target_pos_ = right_target_pos_;
        }
    }
    else // 都没有
    {
        have_target_ = 0;
    }

}

void BtExecutor::refereeInformationCallback(const sentry_msgs::msg::RefereeInformation::SharedPtr referee_information)
{
    game_start_ = referee_information->game_start;
    gameover_time_ = referee_information->gameover_time;
    robot_hp_ = referee_information->robot_hp;
    max_hp_ = referee_information->max_hp;
    bullets_ = referee_information->bullets;
    our_outpost_hp_ = referee_information->our_outpost_hp;
    our_base_hp_ = referee_information->our_base_hp;
    base_shield_ = referee_information->base_shield;
    gold_coins_ = referee_information->gold_coins;

    low_hp_list_.clear();
    for (unsigned int i = 0; i < referee_information->enemy_hp.size(); i++)
    {
        enemy_hp_[i] = referee_information->enemy_hp[i];
        if (enemy_hp_[i] <= 100 && enemy_hp_[i] != 0)
            low_hp_list_.emplace_back(i);
    }

    // 裁判系统无敌方无人机信息
    // air_force_ = referee_information->air_force;
    force_back_ = referee_information->force_back;

    // rmuc
    in_supply_ = getBit(referee_information->rfid_status, 13);
    in_patrol_ = getBit(referee_information->rfid_status, 14);
    mode_ = referee_information->normal_mode;

    // RCLCPP_INFO(get_logger(), "referee in : \n game_start:%d\n gameover_time_:%d\n robot_hp:%d\n max_hp:%d\n bullets:%d\n our_base_hp:%d\n enemy_base_hp:%d", 
    //             static_cast<int>(game_start_), 
    //             static_cast<int>(gameover_time_),
    //             static_cast<int>(robot_hp_),
    //             static_cast<int>(max_hp_),
    //             static_cast<int>(bullets_),
    //             static_cast<int>(our_base_hp_),
    //             static_cast<int>(enemy_hp_[0])
    //             ); 

}

void BtExecutor::rightRmosCallback(const sentry_interfaces::msg::FollowTarget::SharedPtr follow_target)
{
    right_target_ = follow_target->have_target;
    right_target_pos_ = follow_target->target;
    right_priority_ = follow_target->priority;
    // RCLCPP_INFO(get_logger(), "right_target_ : %d, %f, %f", static_cast<int>(right_target_), right_target_pos_.point.x, right_target_pos_.point.y); 
}

void BtExecutor::leftRmosCallback(const sentry_interfaces::msg::FollowTarget::SharedPtr follow_target)
{
    left_target_ = follow_target->have_target;
    left_target_pos_ = follow_target->target;
    left_priority_ = follow_target->priority;
    // RCLCPP_INFO(get_logger(), "left_target_ : %d, %f, %f", static_cast<int>(left_target_), left_target_pos_.point.x, left_target_pos_.point.y); 
}


inline void BtExecutor::setBit(uint32_t& data, int pos)
{
    data |= (static_cast<uint32_t>(1) << pos);
}

inline void BtExecutor::clearBit(uint32_t& data, int pos)
{
    data &= (static_cast<uint32_t>(0) << pos);
}

inline bool BtExecutor::getBit(const uint32_t& data, int pos)
{
    return (data >> pos) & static_cast<uint32_t>(1);
}

void BtExecutor::setBitsRange(uint32_t &data, int start, int end, uint32_t value)
{
    uint32_t mask = ~((~static_cast<uint32_t>(0) << start) & ((static_cast<uint32_t>(1) << (end + 1)) - 1));
    data &= mask;
    
    value <<= start;
    data |= value;
}


} // namespace sentry_bt_executor

#include <rclcpp_components/register_node_macro.hpp>

// Register the component with class_loader.
// This acts as a sort of entry point, allowing the component to be discoverable when its library
// is being loaded into a running process.
RCLCPP_COMPONENTS_REGISTER_NODE(sentry_bt_executor::BtExecutor)
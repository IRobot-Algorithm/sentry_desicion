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
        "rmul_defense_bt_node",
        "rmul_patrol_bt_node",
        "rmul_go_supply_bt_node",
        "set_nav_target_bt_node",
        "set_left_target_bt_node",
        "set_right_target_bt_node",
        "air_force_condition_bt_node",
        "in_supply_condition_bt_node",
        "base_unfolds_condition_bt_node",
        "have_target_condition_bt_node", 
        "low_hp_condition_bt_node", 
        "low_bullets_condition_bt_node", 
        "enough_hp_condition_bt_node", 
        "game_start_condition_bt_node", 
        "game_about_over_condition_bt_node", 
        "outpost_survives_condition_bt_node", 
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
    blackboard_->set<std::chrono::milliseconds>("server_timeout", std::chrono::milliseconds(10000)); 

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
    blackboard_->set<u_int16_t>("gold_coins", gold_coins_);

    // TODO: 更改初始血量
    enemy_hp_.resize(8, 300);
    
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
    blackboard_->set<std::vector<u_int8_t>>("list", list_); // 自瞄目标

    blackboard_->set<bool>("force_back", force_back_); // 强制回家

    blackboard_->set<double>("loop_time", 0);

    blackboard_->set<bool>("in_supply", in_supply_);

    // rmul
    blackboard_->set<double>("supply_time", supply_time_);

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
        double loop_time = rclcpp::Clock().now().seconds() - time_.seconds();
        time_ = rclcpp::Clock().now();

        if (in_supply_ && max_hp_ - robot_hp_ >= 100)
            supply_time_ += loop_time;
        RCLCPP_INFO(get_logger(), "supply_time: %f", supply_time_);

        /* 比赛状态信息 */
        blackboard_->set<bool>("game_start", game_start_);
        blackboard_->set<u_int16_t>("gameover_time", gameover_time_);

        /* 我方机器人信息 */
        blackboard_->set<u_int16_t>("robot_hp", robot_hp_);
        blackboard_->set<u_int16_t>("bullets", bullets_);
        blackboard_->set<u_int16_t>("our_outpost_hp", our_outpost_hp_);
        blackboard_->set<u_int16_t>("our_base_hp", our_base_hp_);
        blackboard_->set<u_int8_t>("base_shield", base_shield_);
        blackboard_->set<u_int16_t>("gold_coins", gold_coins_);

        // TODO: 更改初始血量
        enemy_hp_.resize(8, 300);

        /* 敌方机器人血量及自瞄状态 */
        blackboard_->set<u_int16_t>("base_hp", enemy_hp_[0]);
        blackboard_->set<u_int16_t>("hero_hp", enemy_hp_[1]);
        blackboard_->set<u_int16_t>("engineer_hp", enemy_hp_[2]);
        blackboard_->set<u_int16_t>("infantry3_hp", enemy_hp_[3]);
        blackboard_->set<u_int16_t>("infantry4_hp", enemy_hp_[4]);
        blackboard_->set<u_int16_t>("infantry5_hp", enemy_hp_[5]);
        blackboard_->set<u_int16_t>("sentry_hp", enemy_hp_[6]);
        blackboard_->set<u_int16_t>("outpost_hp", enemy_hp_[7]);

        judgeTarget();

        blackboard_->set<u_int8_t>("have_target", have_target_);
        blackboard_->set<bool>("gimbal", gimbal_);
        blackboard_->set<geometry_msgs::msg::PointStamped>("target_pos", target_pos_);    
        blackboard_->set<std::vector<u_int8_t>>("list", list_); // 自瞄目标

        blackboard_->set<bool>("in_supply", in_supply_);
        blackboard_->set<double>("supply_time", supply_time_);

        blackboard_->set<bool>("force_back", force_back_); // 强制回家


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

    // for rmul 2024
    if (right_id_ == 1 && right_target_ > 0)
    {
        have_target_ = right_target_;
        gimbal_ = 0;
        target_pos_ = right_target_pos_;
    }
    else if (left_id_ == 1 && left_target_ > 0)
    {
        have_target_ = left_target_;
        gimbal_ = 1;
        target_pos_ = left_target_pos_;
    }
    else
    {
        if (right_target_ > 0)
        {
            have_target_ = right_target_;
            gimbal_ = 0;
            target_pos_ = right_target_pos_;
        }
        else if (left_target_ > 0)
        {
            have_target_ = left_target_;
            gimbal_ = 1;
            target_pos_ = left_target_pos_;
        }
        else
            have_target_ = 0;
    }

    // std::vector<u_int8_t> id(8);
    // std::sort(id.begin(), id.end(),
    //       [this](size_t a, size_t b){return enemy_hp_[a] > enemy_hp_[b];});
    // list_ = id;
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

    for (unsigned int i = 0; i < referee_information->enemy_hp.size(); i++)
        enemy_hp_[i] = referee_information->enemy_hp[i];

    // 裁判系统无敌方无人机信息
    // air_force_ = referee_information->air_force;
    force_back_ = referee_information->force_back;

    // rmul
    in_supply_ = referee_information->in_supply;
}

void BtExecutor::rightRmosCallback(const sentry_interfaces::msg::FollowTarget::SharedPtr follow_target)
{
    right_target_ = follow_target->have_target;
    right_target_pos_ = follow_target->target;
    right_id_ = follow_target->priority;
    RCLCPP_INFO(get_logger(), "right_target_ : %d, %f, %f", static_cast<int>(right_target_), right_target_pos_.point.x, right_target_pos_.point.y); 
}

void BtExecutor::leftRmosCallback(const sentry_interfaces::msg::FollowTarget::SharedPtr follow_target)
{
    left_target_ = follow_target->have_target;
    left_target_pos_ = follow_target->target;
    left_id_ = follow_target->priority;
    RCLCPP_INFO(get_logger(), "left_target_ : %d, %f, %f", static_cast<int>(left_target_), left_target_pos_.point.x, left_target_pos_.point.y); 
}


} // namespace sentry_bt_executor

#include <rclcpp_components/register_node_macro.hpp>

// Register the component with class_loader.
// This acts as a sort of entry point, allowing the component to be discoverable when its library
// is being loaded into a running process.
RCLCPP_COMPONENTS_REGISTER_NODE(sentry_bt_executor::BtExecutor)
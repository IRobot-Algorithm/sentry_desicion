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
        "rmul_patrol_bt_node",
        "rmul_go_supply_bt_node",
        "air_force_condition_bt_node",
        "base_unfolds_condition_bt_node",
        "low_hp_bt_node", 
        "game_start_condition_bt_node", 
        "outpost_survives_condition_bt_node", 
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
    blackboard_->set<std::chrono::milliseconds>("server_timeout", std::chrono::milliseconds(50)); 

    /* 哨兵定位信息 */
    blackboard_->set<bool>("in_patrol_area", in_patrol_area_);
    blackboard_->set<bool>("in_supply_area", in_supply_area_);
    blackboard_->set<bool>("in_save_space", in_save_space_);  
    blackboard_->set<double>("leave_time", leave_time_);

    /* 我方机器人信息 */
    blackboard_->set<int>("robot_hp", robot_hp_);
    blackboard_->set<int>("bullets", bullets_);
    blackboard_->set<int>("our_outpost_hp", our_outpost_hp_);
    blackboard_->set<bool>("outpost_survives", outpost_survives_);
    blackboard_->set<int>("our_base_hp", our_base_hp_);
    blackboard_->set<bool>("base_unfolds", base_unfolds_);
    blackboard_->set<int>("gold_coins", gold_coins_);
    
    /* 比赛状态信息 */
    blackboard_->set<bool>("game_start", robot_hp_);
    blackboard_->set<double>("gameover_time", gameover_time_);

    enemy_hp_.resize(14, 300);
    find_armors_.resize(14, false);
    /* 敌方机器人血量及自瞄状态 */
    blackboard_->set<int>("base_hp", enemy_hp_[0]);
    blackboard_->set<int>("hero_hp", enemy_hp_[1]);
    blackboard_->set<int>("engineer_hp", enemy_hp_[2]);
    blackboard_->set<int>("infantry3_hp", enemy_hp_[3]);
    blackboard_->set<int>("infantry4_hp", enemy_hp_[4]);
    blackboard_->set<int>("infantry5_hp", enemy_hp_[5]);
    blackboard_->set<int>("sentry_hp", enemy_hp_[6]);
    blackboard_->set<int>("outpost_hp", enemy_hp_[7]);
    blackboard_->set<int>("balance3_hp", enemy_hp_[11]);
    blackboard_->set<int>("balance4_hp", enemy_hp_[12]);
    blackboard_->set<int>("balance5_hp", enemy_hp_[13]);
        
    blackboard_->set<bool>("air_force", air_force_); // 敌方空中机器人信息

    get_parameter("default_bt_xml_filename", default_bt_xml_filename_);
    std::cout << "default_bt_xml_filename_: " << default_bt_xml_filename_ << std::endl;

    if (!loadBehaviorTree(default_bt_xml_filename_))
    {
        RCLCPP_ERROR(get_logger(), "Error loading XML file: %s", default_bt_xml_filename_.c_str());
        // return
    }

    /* create callback group */ 
    this->odometry_sub_callback_group_ = this->create_callback_group(rclcpp::CallbackGroupType::Reentrant);
    this->referee_information_sub_callback_group_ = this->create_callback_group(rclcpp::CallbackGroupType::Reentrant);
    this->armors_sub_callback_group_ = this->create_callback_group(rclcpp::CallbackGroupType::Reentrant);
    this->execute_timer_callback_group_ = this->create_callback_group(rclcpp::CallbackGroupType::Reentrant);

    /* create subscription */
    auto odometry_sub_options = rclcpp::SubscriptionOptions();
    odometry_sub_options.callback_group = this->odometry_sub_callback_group_;
    odometry_sub_ = this->create_subscription<nav_msgs::msg::Odometry>("/Odometry", 5,
        std::bind(&BtExecutor::odometryCallback, this, _1), odometry_sub_options);

    auto referee_information_sub_options = rclcpp::SubscriptionOptions();
    referee_information_sub_options.callback_group = this->referee_information_sub_callback_group_;
    referee_information_sub_ = this->create_subscription<sentry_interfaces::msg::RefereeInformation>("/referee_information", 10,
        std::bind(&BtExecutor::refereeInformationCallback, this, _1), referee_information_sub_options);

    auto armors_sub_options = rclcpp::SubscriptionOptions();
    armors_sub_options.callback_group = this->armors_sub_callback_group_;
    armors_sub_ = this->create_subscription<rmos_interfaces::msg::Armors>("/armors", 10,
        std::bind(&BtExecutor::armorsCallback, this, _1), armors_sub_options);


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
        judgeArea();
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

void BtExecutor::odometryCallback(const nav_msgs::msg::Odometry::SharedPtr odometry)
{
    sentry_odom_ = *odometry;
}

void BtExecutor::refereeInformationCallback(const sentry_interfaces::msg::RefereeInformation::SharedPtr referee_information)
{


}

void BtExecutor::armorsCallback(const rmos_interfaces::msg::Armors::SharedPtr armors)
{

    // std::fill(find_armors_.begin(), find_armors_.end(), false);

    // for (auto &armor: armors->armors)
    // {
    //     find_armors_[armor.num_id] = true;
    // }

    // blackboard_->set<bool>("find_base", find_armors_[0]);
    // blackboard_->set<bool>("find_hero", find_armors_[1]);
    // blackboard_->set<bool>("find_engineer", find_armors_[2]);
    // blackboard_->set<bool>("find_infantry3", find_armors_[3]);
    // blackboard_->set<bool>("find_infantry4", find_armors_[4]);
    // blackboard_->set<bool>("find_infantry5", find_armors_[5]);
    // blackboard_->set<bool>("find_sentry", find_armors_[6]);
    // blackboard_->set<bool>("find_outpost", find_armors_[7]);
    // blackboard_->set<bool>("find_balance3", find_armors_[11]);
    // blackboard_->set<bool>("find_balance4", find_armors_[12]);
    // blackboard_->set<bool>("find_balance5", find_armors_[13]);

}

void BtExecutor::judgeArea()
{

    double x = sentry_odom_.pose.pose.position.x;
    double y = sentry_odom_.pose.pose.position.y;


    if (x > -2 && x < 2 && y > -1 && y < 1)
    {
        in_patrol_area_ = true;
    }
    else
    {
        if (in_patrol_area_)
        {
            rclcpp::Clock steady_clock{RCL_STEADY_TIME};
            auto time = steady_clock.now();
            leave_time_ = time.seconds();
        }
        in_patrol_area_ = false;
    }

    if (x > 7 && x < 8 && y > -5 && y < -4)
        in_supply_area_ = true;
    else
        in_supply_area_ = false;

    if (y < 5)
        in_save_space_ = true;
    else
        in_save_space_ = false;

    blackboard_->set<bool>("in_patrol_area", in_patrol_area_);
    blackboard_->set<bool>("in_supply_area", in_supply_area_);
    blackboard_->set<bool>("in_save_space", in_save_space_);
    blackboard_->set<double>("leave_time", leave_time_);

}

} // namespace sentry_bt_executor

#include <rclcpp_components/register_node_macro.hpp>

// Register the component with class_loader.
// This acts as a sort of entry point, allowing the component to be discoverable when its library
// is being loaded into a running process.
RCLCPP_COMPONENTS_REGISTER_NODE(sentry_bt_executor::BtExecutor)
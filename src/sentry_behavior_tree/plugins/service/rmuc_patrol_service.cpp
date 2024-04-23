#include <string>
#include "sentry_behavior_tree/plugins/service/rmuc_patrol_service.hpp"
#include <random>

namespace sentry_behavior_tree{

    RmucPatrolService::RmucPatrolService(const std::string & service_node_name,
        const BT::NodeConfiguration & conf)
        : nav2_behavior_tree::BtServiceNode<sentry_srvs::srv::NavGoal>(service_node_name, conf){}

    void RmucPatrolService::on_tick()
    {
        if (rclcpp::Clock().now().seconds() - last_time_.seconds() > 5)
        {
            last_time_ = rclcpp::Clock().now();
            point_ = generateRandomPoint();
        }

        request_->pose.pose.position.x = point_.first;
        request_->pose.pose.position.y = point_.second;

        // RCLCPP_INFO(node_->get_logger(),"rmuc_patrol_service on_tick()... ");
    }

    BT::NodeStatus RmucPatrolService::check_future(
    std::shared_future<sentry_srvs::srv::NavGoal::Response::SharedPtr> future_result)
    {
        rclcpp::FutureReturnCode rc;
        rc = rclcpp::spin_until_future_complete(
        node_,
        future_result, server_timeout_);
        if (rc == rclcpp::FutureReturnCode::SUCCESS)
        {
            auto result = future_result.get();
            if (result->is_arrive)
            {
                last_time_ = rclcpp::Clock().now();
                point_ = generateRandomPoint();
                return BT::NodeStatus::SUCCESS;
            }
            else
                return BT::NodeStatus::FAILURE;
        }
        else if (rc == rclcpp::FutureReturnCode::TIMEOUT)
        {
            RCLCPP_WARN(
                node_->get_logger(),
                "Node timed out while executing service call to %s.", service_name_.c_str());
            on_wait_for_result();
        }
        return BT::NodeStatus::FAILURE;
    }

    std::pair<double, double> RmucPatrolService::generateRandomPoint()
    {
        std::random_device rd;
        std::mt19937 gen(rd());
        
        std::uniform_real_distribution<> dis_x(-1.52, 0.9);
        std::uniform_real_distribution<> dis_y(0.45, 2.2);
        
        double x = dis_x(gen);
        double y = dis_y(gen);
        
        // be trust
        if (x < -1.52)
            x = -1.52;
        else if (x > 0.9)
            x = 0.9;
        if (y < 0.45)
            y = 0.45;
        else if (y > 2.2)
            y = 2.2;

        if (!direction_)
            y = -y;
        direction_ = direction_ ? false : true;

        return std::make_pair(x, y);
    }


}

#include "behaviortree_cpp_v3/bt_factory.h"
BT_REGISTER_NODES(factory)
{
  factory.registerNodeType<sentry_behavior_tree::RmucPatrolService>(
    "RmucPatrol");
    //注意这里""内没有Service 
}
#include <string>
#include "sentry_behavior_tree/plugins/service/rmuc_test_patrol_service.hpp"

namespace sentry_behavior_tree{

    RmucTestPatrolService::RmucTestPatrolService(const std::string & service_node_name,
        const BT::NodeConfiguration & conf)
        : nav2_behavior_tree::BtServiceNode<sentry_srvs::srv::NavGoal>(service_node_name, conf){}

    void RmucTestPatrolService::on_tick()
    {
        // if (rclcpp::Clock().now().seconds() - last_time_.seconds() > 6)
        // {
        //     last_time_ = rclcpp::Clock().now();
        //     id_++;
        // }

        if (id_ > 8)
            id_ = 0;

        switch (id_)
        {
            case 0: // back
            {
                request_->pose.pose.position.x = -0.36;
                request_->pose.pose.position.y = 1.0;
                break;
            }
            case 1: // left
            {
                request_->pose.pose.position.x = 0.0;
                request_->pose.pose.position.y = 2.05;
                break;
            }
            case 2: // right
            {
                request_->pose.pose.position.x = 0.0;
                request_->pose.pose.position.y = -2.05;
                break;
            }
            case 3: // supply
            {
                request_->pose.pose.position.x = -4.2;
                request_->pose.pose.position.y = -6.1;
                break;
            }
            case 4: // enemyoutpost
            {
                request_->pose.pose.position.x = 6.33;
                request_->pose.pose.position.y = 6.85;
                break;
            }
            case 5: // enemyarea
            {
                request_->pose.pose.position.x = 12.6;
                request_->pose.pose.position.y = -3.2;
                break;
            }
            case 6: // enemy
            {
                request_->pose.pose.position.x = 17.2;
                request_->pose.pose.position.y = -2.2;
                break;
            }
            case 7: // enemy
            {
                request_->pose.pose.position.x = 14.6;
                request_->pose.pose.position.y = 2.2;
                break;
            }
            default: // enemyslope
            {
                request_->pose.pose.position.x = 5.6;
                request_->pose.pose.position.y = 6.93;
                break;
            }

        }   
        // RCLCPP_INFO(node_->get_logger(),"rmuc_test_patrol_service on_tick()... ");
    }

    BT::NodeStatus RmucTestPatrolService::check_future(
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
                id_++;
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


}

#include "behaviortree_cpp_v3/bt_factory.h"
BT_REGISTER_NODES(factory)
{
  factory.registerNodeType<sentry_behavior_tree::RmucTestPatrolService>(
    "RmucTestPatrol");
    //注意这里""内没有Service 
}
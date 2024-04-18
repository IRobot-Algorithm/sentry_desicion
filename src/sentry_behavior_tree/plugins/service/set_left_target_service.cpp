#include <string>
#include "sentry_behavior_tree/plugins/service/set_left_target_service.hpp"

namespace sentry_behavior_tree{

    SetLeftTargetService::SetLeftTargetService(const std::string & service_node_name,
        const BT::NodeConfiguration & conf)
        : nav2_behavior_tree::BtServiceNode<sentry_interfaces::srv::AimTarget>(service_node_name, conf){}

    void SetLeftTargetService::on_tick()
    {
        std::vector<u_int8_t> low_hp_list;
        getInput("low_hp_list", low_hp_list);

        std::string list_name;
        if (!getInput<std::string>("list_name", list_name))
            RCLCPP_WARN(node_->get_logger(),"Can not get list name !!!");
        std::vector<uint8_t> list = getList(list_name);

        size_t i = 0;
        for (auto it1 = list.begin(); it1 != list.end();)
        {
            auto it2 = std::find(low_hp_list.begin(), low_hp_list.end(), *it1);
            if (it2 != low_hp_list.end())
            {
                if (i < low_hp_list.size())
                {
                    std::swap(*(low_hp_list.begin() + i), *it2);
                    i++;
                }
                it1 = list.erase(it1);
            }
            else
            {
                it1++;
            }
        }
        list.insert(list.begin(), low_hp_list.begin(), low_hp_list.end());

        request_->list = list;

        // RCLCPP_INFO(node_->get_logger(),"set_left_target_service on_tick()... ");
    }

    BT::NodeStatus SetLeftTargetService::check_future(
    std::shared_future<sentry_interfaces::srv::AimTarget::Response::SharedPtr> future_result)
    {
        rclcpp::FutureReturnCode rc;
        rc = rclcpp::spin_until_future_complete(
        node_,
        future_result, server_timeout_);
        if (rc == rclcpp::FutureReturnCode::SUCCESS)
            return BT::NodeStatus::SUCCESS;
        else if (rc == rclcpp::FutureReturnCode::TIMEOUT)
        {
            RCLCPP_WARN(
                node_->get_logger(),
                "Node timed out while executing service call to %s.", service_name_.c_str());
            on_wait_for_result();
        }
        return BT::NodeStatus::FAILURE;
    }

    BT::PortsList SetLeftTargetService::providedPorts()
    {
        return providedBasicPorts(
        {
            BT::InputPort<std::vector<u_int8_t>>("low_hp_list"),
        });
    }

    std::vector<uint8_t> SetLeftTargetService::getList(const std::string& list_name)
    {
        if (list_name == "All")
            return {1, 6, 3, 4, 5, 2};
        else if (list_name == "ExceptSentry")
            return {1, 3, 4, 5, 2};
        else if (list_name == "None")
            return {};
        else if (list_name == "OnlyOutpost")
            return {7};
        else
        {
            return {100}; // ERROR!!!
            RCLCPP_WARN(node_->get_logger(), "ERROR AIMMING TARGET!!!");
        }
    }

}

#include "behaviortree_cpp_v3/bt_factory.h"
BT_REGISTER_NODES(factory)
{
  factory.registerNodeType<sentry_behavior_tree::SetLeftTargetService>(
    "SetLeftTarget");
    //注意这里""内没有Service 
}
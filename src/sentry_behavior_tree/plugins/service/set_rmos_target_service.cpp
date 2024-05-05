#include <string>
#include "sentry_behavior_tree/plugins/service/set_rmos_target_service.hpp"

namespace sentry_behavior_tree{

    SetRmosTargetService::SetRmosTargetService(const std::string & service_node_name,
        const BT::NodeConfiguration & conf)
        : nav2_behavior_tree::BtServiceNode<sentry_interfaces::srv::AimTarget>(service_node_name, conf){}

    void SetRmosTargetService::on_tick()
    {
        std::vector<u_int8_t> low_hp_list, invincibility_list;
        getInput("low_hp_list", low_hp_list);
        getInput("invincibility_list", invincibility_list);

        u_int16_t gameover_time;
        getInput("gameover_time", gameover_time);

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

        if (gameover_time < 240 && list_name != "OnlyOutpost" && list_name != "None") // 三分钟后 打工程
            list.push_back(2);

        std::unordered_set<int> inv_set(invincibility_list.begin(), invincibility_list.end());
        list.erase(
            std::remove_if(
                list.begin(),
                list.end(),
                [&inv_set](int x) { return inv_set.count(x) > 0; }
            ),
            list.end()
        );

        request_->list = list;


        // RCLCPP_INFO(node_->get_logger(), "low_hp_list:\n");
        // for (size_t i = 0; i < low_hp_list.size(); i++)
        //     RCLCPP_INFO(node_->get_logger(), "%d ", static_cast<int>(low_hp_list[i]));

        // RCLCPP_INFO(node_->get_logger(), "invincibility_list:\n");
        // for (size_t i = 0; i < invincibility_list.size(); i++)
        //     RCLCPP_INFO(node_->get_logger(), "%d ", static_cast<int>(invincibility_list[i]));

        // RCLCPP_INFO(node_->get_logger(), "list:\n");
        // for (size_t i = 0; i < list.size(); i++)
        //     RCLCPP_INFO(node_->get_logger(), "%d ", static_cast<int>(list[i]));

        // RCLCPP_INFO(node_->get_logger(),"set_rmos_target_service on_tick()... ");
    }

    BT::NodeStatus SetRmosTargetService::check_future(
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

    BT::PortsList SetRmosTargetService::providedPorts()
    {
        return providedBasicPorts(
        {
            BT::InputPort<std::string>("list_name"),
            BT::InputPort<std::vector<u_int8_t>>("low_hp_list"),
            BT::InputPort<std::vector<u_int8_t>>("invincibility_list"),
            BT::InputPort<u_int16_t>("gameover_time"),
        });
    }

    std::vector<uint8_t> SetRmosTargetService::getList(const std::string& list_name)
    {
        if (list_name == "All")
            return {1, 6, 3, 4, 5};
        else if (list_name == "ExceptSentry")
            return {1, 3, 4, 5};
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
  factory.registerNodeType<sentry_behavior_tree::SetRmosTargetService>(
    "SetRmosTarget");
    //注意这里""内没有Service 
}
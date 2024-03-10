#ifndef SENTRY_BEHAVIOR_TREE__PLUGINS__SERVICE__SET_RIGHT_TARGET_HPP_
#define SENTRY_BEHAVIOR_TREE__PLUGINS__SERVICE__SET_RIGHT_TARGET_HPP_

#include "nav2_behavior_tree/bt_service_node.hpp"
#include "sentry_interfaces/srv/aim_target.hpp"
#include "geometry_msgs/msg/point_stamped.hpp"

namespace sentry_behavior_tree{

/*
* @brief 设置左云台自瞄目标
* @auther wyq
*/
class SetRightTargetService : public nav2_behavior_tree::BtServiceNode<sentry_interfaces::srv::AimTarget>{

    public:
        SetRightTargetService(const std::string & service_node_name,
            const BT::NodeConfiguration & conf);

        void on_tick() override;

        BT::NodeStatus check_future(
        std::shared_future<sentry_interfaces::srv::AimTarget::Response::SharedPtr> future_result) override;
    
        static BT::PortsList providedPorts();
    private:
};


}  // namespace sentry_behavior_tree


#endif
#ifndef SENTRY_BEHAVIOR_TREE__PLUGINS__SERVICE__SET_HALF_SCAN_HPP_
#define SENTRY_BEHAVIOR_TREE__PLUGINS__SERVICE__SET_HALF_SCAN_HPP_

#include "nav2_behavior_tree/bt_service_node.hpp"
#include "sentry_interfaces/srv/half_scan.hpp"
#include "geometry_msgs/msg/point_stamped.hpp"

namespace sentry_behavior_tree{

/*
* @brief 设置扫描范围为90度
* @auther wyq
*/
class SetHalfScanService : public nav2_behavior_tree::BtServiceNode<sentry_interfaces::srv::HalfScan>{

    public:
        SetHalfScanService(const std::string & service_node_name,
            const BT::NodeConfiguration & conf);

        void on_tick() override;

        BT::NodeStatus check_future(
        std::shared_future<sentry_interfaces::srv::HalfScan::Response::SharedPtr> future_result) override;
    
    private:
};


}  // namespace sentry_behavior_tree


#endif
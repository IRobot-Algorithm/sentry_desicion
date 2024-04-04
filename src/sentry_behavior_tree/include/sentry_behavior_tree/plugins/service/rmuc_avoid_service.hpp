#ifndef SENTRY_BEHAVIOR_TREE__PLUGINS__SERVICE__RMUC_AVOID_HPP_
#define SENTRY_BEHAVIOR_TREE__PLUGINS__SERVICE__RMUC_AVOID_HPP_

#include "nav2_behavior_tree/bt_service_node.hpp"
#include "sentry_srvs/srv/nav_goal.hpp"

namespace sentry_behavior_tree{

/*
* @brief rmuc躲避巡逻
* @auther wyq
*/
class RmucAvoidService : public nav2_behavior_tree::BtServiceNode<sentry_srvs::srv::NavGoal>{

    public:
        RmucAvoidService(const std::string & service_node_name,
            const BT::NodeConfiguration & conf);

        void on_tick() override;

        BT::NodeStatus check_future(
        std::shared_future<sentry_srvs::srv::NavGoal::Response::SharedPtr> future_result) override;
    
    private:
        int avoid_id_ = 0;
        rclcpp::Time last_time_;
};


}  // namespace sentry_behavior_tree


#endif
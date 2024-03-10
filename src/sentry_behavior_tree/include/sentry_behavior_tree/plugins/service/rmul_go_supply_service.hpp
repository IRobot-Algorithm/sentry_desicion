#ifndef SENTRY_BEHAVIOR_TREE__PLUGINS__SERVICE__RMUL_GO_SUPPLY_HPP_
#define SENTRY_BEHAVIOR_TREE__PLUGINS__SERVICE__RMUL_GO_SUPPLY_HPP_

#include "nav2_behavior_tree/bt_service_node.hpp"
#include "sentry_srvs/srv/nav_goal.hpp"

namespace sentry_behavior_tree{

/*
* @brief 前往rmul补给区
* @auther wyq
*/
class RmulGoSupplyService : public nav2_behavior_tree::BtServiceNode<sentry_srvs::srv::NavGoal>{

    public:
        RmulGoSupplyService(const std::string & service_node_name,
            const BT::NodeConfiguration & conf);

        void on_tick() override;

        BT::NodeStatus check_future(
        std::shared_future<sentry_srvs::srv::NavGoal::Response::SharedPtr> future_result) override;

        static BT::PortsList providedPorts();
    private:
};


}  // namespace sentry_behavior_tree


#endif
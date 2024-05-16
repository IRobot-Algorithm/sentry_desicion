#ifndef SENTRY_BEHAVIOR_TREE__PLUGINS__SERVICE__SET_OUTPOST_TARGET_HPP_
#define SENTRY_BEHAVIOR_TREE__PLUGINS__SERVICE__SET_OUTPOST_TARGET_HPP_

#include "nav2_behavior_tree/bt_service_node.hpp"
#include "sentry_srvs/srv/nav_target.hpp"
#include "geometry_msgs/msg/point_stamped.hpp"

namespace sentry_behavior_tree{

/*
* @brief 指向敌方前哨站目标
* @auther wyq
*/
class SetOutpostTargetService : public nav2_behavior_tree::BtServiceNode<sentry_srvs::srv::NavTarget>{

    public:
        SetOutpostTargetService(const std::string & service_node_name,
            const BT::NodeConfiguration & conf);

        void on_tick() override;

        BT::NodeStatus check_future(
        std::shared_future<sentry_srvs::srv::NavTarget::Response::SharedPtr> future_result) override;
    
    private:
};


}  // namespace sentry_behavior_tree


#endif
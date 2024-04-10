#ifndef SENTRY_BEHAVIOR_TREE__PLUGINS__SERVICE__BUY_BULLETS_HPP_
#define SENTRY_BEHAVIOR_TREE__PLUGINS__SERVICE__BUY_BULLETS_HPP_

#include "nav2_behavior_tree/bt_service_node.hpp"
#include "sentry_srvs/srv/buy_bullets.hpp"
#include "geometry_msgs/msg/point_stamped.hpp"

namespace sentry_behavior_tree{

/*
* @brief 购买弹丸
* @auther wyq
*/
class BuyBulletsService : public nav2_behavior_tree::BtServiceNode<sentry_srvs::srv::BuyBullets>{

    public:
        BuyBulletsService(const std::string & service_node_name,
            const BT::NodeConfiguration & conf);

        void on_tick() override;

        BT::NodeStatus check_future(
        std::shared_future<sentry_srvs::srv::BuyBullets::Response::SharedPtr> future_result) override;
    
        static BT::PortsList providedPorts();
    private:
};


}  // namespace sentry_behavior_tree


#endif
#ifndef SENTRY_BEHAVIOR_TREE__PLUGINS__SERVICE__RMUC_GO_ENEMY_OUTPOST_HPP_
#define SENTRY_BEHAVIOR_TREE__PLUGINS__SERVICE__RMUC_GO_ENEMY_OUTPOST_HPP_

#include "nav2_behavior_tree/bt_service_node.hpp"
#include "sentry_srvs/srv/nav_goal.hpp"

namespace sentry_behavior_tree{

/*
* @brief 前往rmuc敌方前哨站
* @auther wyq
*/
class RmucGoEnemyOutpostService : public nav2_behavior_tree::BtServiceNode<sentry_srvs::srv::NavGoal>{

    public:
        RmucGoEnemyOutpostService(const std::string & service_node_name,
            const BT::NodeConfiguration & conf);

        void on_tick() override;

        BT::NodeStatus check_future(
        std::shared_future<sentry_srvs::srv::NavGoal::Response::SharedPtr> future_result) override;

    private:
};


}  // namespace sentry_behavior_tree


#endif
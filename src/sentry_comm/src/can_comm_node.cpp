#include <vector>
#include <Eigen/Dense>
#include "sentry_comm/comm_node.hpp"

using namespace std::chrono;

namespace sentry_comm
{


CanCommNode::CanCommNode(const rclcpp::NodeOptions &options) : CommNode("sentry_comm", options)
{

  // create callback group
  this->send_infomation_callback_group_ = this->create_callback_group(rclcpp::CallbackGroupType::Reentrant);

  this->referee_information_timer_ = this->create_wall_timer
                          (100ms, std::bind(&CanCommNode::sendRefereeInformationCallBack, this), send_infomation_callback_group_);

  this->recevie_thread_ = std::thread(&CanCommNode::recevieCallBack, this);

}

CanCommNode::~CanCommNode()
{
	if (recevie_thread_.joinable())
				recevie_thread_.join();
}

void CanCommNode::recevieCallBack()
{

  while(rclcpp::ok())
  {
    uint id = 0;
    u_char buf[8] = {0};
    u_char dlc = 0;

    this->can_.receive(id, buf, dlc);

    switch(id)
		{

      case COMPETITION_RECEVIE_ID:
      {

    		int progress = (int)(buf[0] % 8);

        if(progress == 4)
        	referee_information_.game_start = true;

        break;
      }


      case ROBOT_HP_ID:
      {
          u_int16_t red_outpost ,blue_outpost;
          memcpy(&red_outpost, buf, 2);
          memcpy(&blue_outpost, &buf[2], 2);
          referee_information_.red_outpost_hp = red_outpost;
          referee_information_.blue_outpost_hp = blue_outpost;
          break;
      }

      default:
        break;

      std::this_thread::sleep_for(std::chrono::microseconds(10));
		}
  }

}

void CanCommNode::sendRefereeInformationCallBack()
{
  referee_information_pub_->publish(referee_information_);
}


} // namespace sentry_comm

#include <rclcpp_components/register_node_macro.hpp>
// Register the component with class_loader.
// This acts as a sort of entry point, allowing the component to be discoverable when its library
// is being loaded into a running process.
RCLCPP_COMPONENTS_REGISTER_NODE(sentry_comm::CanCommNode)
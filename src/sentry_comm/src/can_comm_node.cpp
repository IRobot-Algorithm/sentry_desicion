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

  this->referee_information_pub_ = this->create_publisher<sentry_msgs::msg::RefereeInformation>(
              "/referee_information", 10);

  this->recevie_thread_ = std::thread(&CanCommNode::recevieCallBack, this);

  referee_information_.game_start = false;
  referee_information_.air_force = false; // 裁判系统没有这个信息
  referee_information_.force_back = false;
  referee_information_.enemy_hp.resize(8);
  
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
    		int progress = (int)(buf[0] & 0xf0 >> 4);
        if (progress == 4)
        	referee_information_.game_start = true;
        else
        	referee_information_.game_start = false;
        u_int16_t gameover_time;
        memcpy(&gameover_time, &buf[1], 2);
        referee_information_.gameover_time = gameover_time;
        break;
      }
      case ROBOT_STATE_RECEIVE_ID:
      {
        if (dlc == 6)
        {
          if (buf[0] < 10)
              my_color_ = 0; //red
          else if (buf[0] > 100)
              my_color_ = 1; //blue 

          u_int16_t robot_hp, max_hp;
          memcpy(&robot_hp, &buf[2], 2);
          memcpy(&max_hp, &buf[4], 2);
          referee_information_.robot_hp = robot_hp;
          referee_information_.max_hp = max_hp;
        }
        break;
      }
      case ARCHITECTRUE_HP_RECEIVE_ID:
      {
        if (my_color_ == -1)
          break;
        
        u_int16_t hp[4]; // red outpost / blue outpost / red base / blue base
        memcpy(hp, buf, 8);
        if (my_color_ == 0) // red
        {
          referee_information_.our_outpost_hp = hp[0];
          referee_information_.our_base_hp = hp[2];
          referee_information_.enemy_hp[7] = hp[1];
          referee_information_.enemy_hp[0] = hp[3];
        }
        else if (my_color_ == 1) // blue
        {
          referee_information_.our_outpost_hp = hp[1];
          referee_information_.our_base_hp = hp[3];
          referee_information_.enemy_hp[7] = hp[0];
          referee_information_.enemy_hp[0] = hp[2];
        }
        break;
      }
      case ECONOMY_RECEIVE_ID:
      {
        u_int16_t eco[2]; // 发弹量 剩余金币
        memcpy(eco, buf, 4);
        referee_information_.bullets = eco[0];
        referee_information_.gold_coins = eco[1];
        break;
      }
      case ENEMY_HP_RECEIVE_ID:
      {
        u_int16_t hp[4]; // hero / infantry 1 2 3
        memcpy(hp, buf, 8);
        referee_information_.enemy_hp[1] = hp[0];
        referee_information_.enemy_hp[3] = hp[1];
        referee_information_.enemy_hp[4] = hp[2];
        referee_information_.enemy_hp[5] = hp[3];
        break;
      }
      case RFID_RECEIVE_ID:
      {
        // referee_information_.in_supply = (buf[0] & 0x2000) == 0x2000;
        break;
      }
      case CLIENT_RECEIVE_ID:
      {
        // TODO: keyward force back

        referee_information_.base_shield = buf[5];
      }
      default:
        break;

      std::this_thread::sleep_for(std::chrono::microseconds(10));
		}
  }

}

void CanCommNode::sendRefereeInformationCallBack()
{
  if (my_color_ != -1)
    referee_information_pub_->publish(referee_information_);
}


} // namespace sentry_comm

#include <rclcpp_components/register_node_macro.hpp>
// Register the component with class_loader.
// This acts as a sort of entry point, allowing the component to be discoverable when its library
// is being loaded into a running process.
RCLCPP_COMPONENTS_REGISTER_NODE(sentry_comm::CanCommNode)
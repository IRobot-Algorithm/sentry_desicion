#ifndef RMOS_COMM__CAN_HPP_
#define RMOS_COMM__CAN_HPP_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <net/if.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <linux/can.h>
#include <linux/can/raw.h>
#include <fcntl.h>
#include <chrono>
#include <iostream>

#define CAN_NAME "can0"

#define ROBOT_RECEIVE_ID 0x129       // 机器人状态信息ID
#define ROBOT_RECEIVE_ID 0x129       // 机器人状态信息ID
#define COMPETITION_RECEVIE_ID 0x12B // 比赛信息ID

#define CLIENT_RECEIVE_ID 0x134     // 由云台手通过客户端发送的信息
#define ROBOT_HP_ID 0x135

namespace transporter
{
    class Can
    {
    private:
        int socket_fd;
        struct sockaddr_can addr;
        struct ifreq interface_request;

    public:
        /**
         *  @brief  接收数据，根据id区分数据包，需要大于1000Hz频率接收
         *  @return error_code
         */
        int receive(uint &id, u_char *buf, u_char &dlc);

        /**
         * @brief   发送数据
         * @param   id  数据对应的ID
         * @param   buf 数据，长度小于等于8
         * @param   dlc 数据长度
         * @return  error_code
         */
        int send(uint id, u_char *buf, u_char dlc);

        Can();
        ~Can();

        enum CanError
        {
            SUCCESS,
            DLC_ERROR,
            WRITE_ERROR,
            READ_ERROR,
            TIME_ERROR
        };
    };

} // namespace transporter

#endif
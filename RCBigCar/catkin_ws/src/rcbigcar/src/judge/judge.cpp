#include <ros/ros.h>
#include <vector>
#include "serial/serial.h"
#include "rm_protocol.h"
#include "config.h"

void RMUnpackCallback(uint8_t* data) {
    data_frame_unpack_struct *unpack = (data_frame_unpack_struct *)data;
    switch(unpack->cmd_id) {
        case GAME_STATUS_CMD_ID:
            summer_camp_info_t *info = (summer_camp_info_t *)unpack->cmd_data;
        break;
    }
}

int main(int argc, char **argv)
{
	ros::init(argc, argv, "judge");

	ros::NodeHandle nh;

    //Start spinner thread
    ros::AsyncSpinner spinner(1);
    spinner.start();

    //open serial
    serial::Serial serial;
    serial::Timeout timeout = serial::Timeout::simpleTimeout(JUDGE_SERIAL_TIMEOUT);

    serial.setPort(JUDGE_SERIAL_NAME);
    serial.setBaudrate(JUDGE_SERIAL_BAUDRATE);
    serial.setTimeout(timeout);

    while(!serial.isOpen()) {
        try {
            serial.open();
        }
        catch(const std::exception& e) {
            ROS_WARN("Judge Serial cannot open [%s].", e.what());
        }

        ros::Duration(0.5).sleep();
    }

    //Init unpack obj
    unpack_data_t unpack_obj;
    unpack_obj.index = 0;
    unpack_obj.unpack_step = STEP_HEADER_SOF;

    while(ros::ok()) {
        if(serial.waitReadable()) {
            //read to buffer
            std::vector<uint8_t> buffer;
            serial.read(buffer, serial.available());

            //unpack
            for(int idx = 0; idx < buffer.size(); idx++) {
                RMProtocolUnpack(&unpack_obj, buffer[idx], &RMUnpackCallback);
            }
        }

        ros::Duration(1.0 / 100.0).sleep();
    }

	return 0;
}
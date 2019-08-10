#include <ros/ros.h>
#include <vector>
#include "serial/serial.h"
#include "rm_protocol.h"
#include "config.h"
#include <rcbigcar/board.h>

ros::Publisher board_pub;

void RMUnpackCallback(uint8_t* data) {
    data_frame_unpack_struct *unpack = (data_frame_unpack_struct *)data;
    switch(unpack->cmd_id) {
        case GAME_STATUS_CMD_ID:
            summer_camp_info_t *info = (summer_camp_info_t *)unpack->cmd_data;

            rcbigcar::board board;

            //round info
            board.team = info->round_team;
            board.time_left = info->round_remain_tick;
            board.move_left = info->round_remain_cnt;

            board.score.resize(2);
            board.score[0] = info->realtime_score[0];
            board.score[1] = info->realtime_score[1];

            //robot info
            board.robot_x.resize(2);
            board.robot_y.resize(2);

            board.robot_x[0] =  info->car_location[0] & 0xF;
            board.robot_y[0] = (info->car_location[0] >> 4) & 0xF;

            board.robot_x[1] =  info->car_location[1] & 0xF;
            board.robot_y[1] = (info->car_location[1] >> 4) & 0xF;

            //castle info
            board.castle.resize(7);
            for(int i = 0; i < 7; i++) {
                board.castle[i] = info->castle_energy[i].energy[0] - info->castle_energy[i].energy[1];
            }

            //cell info
            board.cell_status.resize(7 * 9);

            int id = 0;
            for(int x = 0; x < 7; x++) for(int y = 0; y < 9; y++) {
                int8_t status = -1;

                //if occupied
                if((info->region_occupy[y][x].status != 0) && (info->region_occupy[y][x].belong != 2)) {

                    //red or blue
                    status = (info->region_occupy[y][x].belong == 1) ? 0 : 1;
                }

                board.cell_status[id++] = status;
            }

            board_pub.publish(board);
        break;
    }
}

int main(int argc, char **argv)
{
	ros::init(argc, argv, "judge");

	ros::NodeHandle nh;

    //init comm
    board_pub = nh.advertise<rcbigcar::board> ("board", 100);

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
                //printf("%2x ", buffer[idx]);
                RMProtocolUnpack(&unpack_obj, buffer[idx], &RMUnpackCallback);
            }
        }

        ros::Duration(1.0 / 100.0).sleep();
    }

	return 0;
}
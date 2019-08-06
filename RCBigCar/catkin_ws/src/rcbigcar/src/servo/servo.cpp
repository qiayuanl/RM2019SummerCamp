#include <ros/ros.h>
#include <tf/tf.h>

#include <std_msgs/Float64MultiArray.h>

#include "serial/serial.h"

ros::Subscriber servo_sub;
serial::Serial servo_serial;

#define VAL_LIMIT(x, minv, maxv) std::min(maxv, std::max(x, minv))

void CallbackServo(const std_msgs::Float64MultiArray::ConstPtr &servo_angle) {
    std::stringstream ss;

    ss << "{";
    for(int i = 0; i < servo_angle->data.size(); i++) {
        double angle = VAL_LIMIT(servo_angle->data[i], 0.0, 1.0);

        int servo_angle = (int)(500.0 + 2000.0 * angle);
        ss << "#" << std::setfill('0') << std::setw(3) << i << "P" << std::setw(4) << servo_angle << "T0000!";
    }
    ss << "}";

    servo_serial.write(ss.str());
}

int main(int argc, char **argv)
{
	ros::init(argc, argv, "robot");

	ros::NodeHandle nh;

    //Read serial paramters
    std::string SerialDevice = nh.param<std::string>("device", "/dev/ttyUSB0");
    int         SerialBaudRate = nh.param<int>        ("baudrate", 115200);

    //Open serial
    serial::Timeout timeout = serial::Timeout::simpleTimeout(100);

    servo_serial.setPort(SerialDevice.c_str());
    servo_serial.setBaudrate(SerialBaudRate);
    servo_serial.setTimeout(timeout);

    while(!servo_serial.isOpen()) {
        try {
            servo_serial.open();
        }
        catch(const std::exception& e) {
            ROS_WARN("Servo Serial cannot open [%s].", e.what());
        }

        ros::Duration(0.5).sleep();
    }

    servo_sub = nh.subscribe<std_msgs::Float64MultiArray>("servo", 100, CallbackServo);

    ros::spin();
	return 0;
}
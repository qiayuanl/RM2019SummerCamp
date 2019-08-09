#include <ros/ros.h>

#include <geometry_msgs/Twist.h>
#include <sensor_msgs/Joy.h>
#include <dynamic_reconfigure/server.h>

#include "rcbigctl/TeleopConfig.h"

rcbigctl::TeleopConfig config;

ros::Publisher twist_pub;
ros::Subscriber joy_sub;

geometry_msgs::Twist twist;

void callback_joy(const sensor_msgs::Joy::ConstPtr &joy)
{
    twist.angular.z = joy->axes[0] * config.MaxW;

    twist.linear.x = joy->axes[4] * config.MaxX;
    twist.linear.y = joy->axes[3] * config.MaxY;
}

void callback_param( rcbigctl::TeleopConfig &_config, uint32_t level ) {
    config = _config;
}

int main(int argc, char **argv)
{
    ros::init(argc, argv, "teleop");

    ros::NodeHandle nh;

    twist_pub = nh.advertise<geometry_msgs::Twist>("velocity", 50);
    joy_sub = nh.subscribe<sensor_msgs::Joy>("joy", 50, &callback_joy);

    //init twist
    twist.angular.x = 0;
    twist.angular.y = 0;
    twist.angular.z = 0;

    twist.linear.x = 0;
    twist.linear.y = 0;
    twist.linear.z = 0;

    //load paramter
    nh.param<double>("MaxX", config.MaxX, 1.0);
    nh.param<double>("MaxY", config.MaxY, 0.0);
    nh.param<double>("MaxW", config.MaxW, 0.0);

    //dynamic reconfigure
    dynamic_reconfigure::Server<rcbigctl::TeleopConfig>               param_server;
    param_server.setCallback(callback_param);

    ros::Rate loop_rate(200);

    while (ros::ok())
    {
        twist_pub.publish(twist);

        ros::spinOnce();
        loop_rate.sleep();
    }
}

#include <cmath>
#include <algorithm>

#include <ros/ros.h>
#include <tf/tf.h>
#include <tf/transform_listener.h>

#include <geometry_msgs/Twist.h>
#include <geometry_msgs/Pose.h>
#include <nav_msgs/Odometry.h>
#include <nav_msgs/Path.h>
#include <std_msgs/Float64MultiArray.h>

#include <dynamic_reconfigure/server.h>
#include "simple_controller/controllerConfig.h"


int main(int argc, char **argv)
{
	ros::init(argc, argv, "controller");

	ros::NodeHandle nh;

	//Create Nodes
	ros::Rate loop_rate(100);

	//Process Jobs
	while (ros::ok())
	{
		ros::spinOnce();
		loop_rate.sleep();
	}

	//Release Nodes
	return 0;
}
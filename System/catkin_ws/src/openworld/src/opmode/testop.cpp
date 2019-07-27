#include <ros/ros.h>
#include <geometry_msgs/Twist.h>

int main(int argc, char **argv) {
	ros::init(argc, argv, "testop");

	ros::NodeHandle nh;
	ros::Publisher twist_pub;

	twist_pub = nh.advertise<geometry_msgs::Twist>("velocity", 50);

	ros::Rate loop_rate(50);
	ros::Time t = ros::Time::now();

	geometry_msgs::Twist twist;
	twist.linear.x = 1.0;

	while (ros::ok())
	{
		if((ros::Time::now() - t).toSec() >= 5) {
			t = ros::Time::now();

			twist.linear.x *= -1;
		}

		twist_pub.publish(twist);

		ros::spinOnce();
		loop_rate.sleep();
	}
}
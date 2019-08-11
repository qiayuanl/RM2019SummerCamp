#include <ros/ros.h>
#include <std_msgs/Float64MultiArray.h>

int main(int argc, char **argv)
{
	ros::init(argc, argv, "testpid");

	ros::NodeHandle nh;
	std_msgs::Float64MultiArray data;

	ros::Publisher data_pub = nh.advertise<std_msgs::Float64MultiArray>("motor", 50);

	ros::Rate loop_rate(50);
	ros::Time t = ros::Time::now();

	bool is_on = true;

	data.data.resize(4);
	while (ros::ok())
	{
		if ((ros::Time::now() - t).toSec() >= 0.7)
		{
			t = ros::Time::now();

			is_on = !is_on;
			for (int i = 0; i < data.data.size(); i++)
				data.data[i] = is_on ? -1.1 : -1.9;
		}

		data_pub.publish(data);

		ros::spinOnce();
		loop_rate.sleep();
	}
}

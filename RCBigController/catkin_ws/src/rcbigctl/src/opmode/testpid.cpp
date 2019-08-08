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

	double value = -8.0 * M_PI;
	bool is_on = true;

	data.data.resize(4);
	for (int i = 0; i < data.data.size(); i++)
		data.data[i] = value;

	while (ros::ok())
	{
		if ((ros::Time::now() - t).toSec() >= 5.0)
		{
			t = ros::Time::now();

			is_on = !is_on;
			for (int i = 0; i < data.data.size(); i++)
				data.data[i] = is_on ? value : 0;
		}

		data_pub.publish(data);

		ros::spinOnce();
		loop_rate.sleep();
	}
}
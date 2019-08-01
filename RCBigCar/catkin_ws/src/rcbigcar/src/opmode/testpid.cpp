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

	data.data.resize(4);
	for (int i = 0; i < data.data.size(); i++)
		data.data[i] = M_PI;

	while (ros::ok())
	{
		if ((ros::Time::now() - t).toSec() >= 1.5)
		{
			t = ros::Time::now();

			for (int i = 0; i < data.data.size(); i++)
				data.data[i] *= -1.0;
		}

		data_pub.publish(data);

		ros::spinOnce();
		loop_rate.sleep();
	}
}
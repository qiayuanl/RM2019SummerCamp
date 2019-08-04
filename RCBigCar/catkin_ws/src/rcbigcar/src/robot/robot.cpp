#include "chassis.h"
#include "motion.h"
#include "hardware.h"

int main(int argc, char **argv)
{
	ros::init(argc, argv, "robot");

	ros::NodeHandle nh;

	//Create Nodes
	Chassis chassis;
	Motion motion;

	ros::Rate loop_rate(ROBOT_SAMPLING_RATE);

	//Process Jobs
	while (ros::ok())
	{
		//Update Subnodes
		chassis.update();
		motion.update();

		//Update Hardware
		Hardware()->update();

		//Spin
		ros::spinOnce();
		loop_rate.sleep();
	}

	//Release Nodes

	//Release Hardware
	ReleaseHardware();
	return 0;
}
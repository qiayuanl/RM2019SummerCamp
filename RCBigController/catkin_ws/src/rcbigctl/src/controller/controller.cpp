#include "positionctl.h"

int main(int argc, char **argv)
{
	ros::init(argc, argv, "controller");

	ros::NodeHandle nh;

	//Create Nodes
	PositionCtl positionctl;

	ros::Rate loop_rate(100);

	//Process Jobs
	while (ros::ok())
	{
		//Update Subnodes
		positionctl.update();

		//Spin
		ros::spinOnce();
		loop_rate.sleep();
	}

	//Release Nodes

	return 0;
}
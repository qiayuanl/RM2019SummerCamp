#include "vlocalization.h"

int main(int argc, char **argv)
{
	ros::init(argc, argv, "vloc");

	ros::NodeHandle nh;

	//Create Nodes
	VLocalization vlocalization;

	ros::Rate loop_rate(200);

	//Process Jobs
	while(ros::ok()) {
		vlocalization.update();

		ros::spinOnce();
		loop_rate.sleep();
	}

	//Release Nodes
	return 0;
}
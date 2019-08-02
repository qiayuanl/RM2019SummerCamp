#include "vlocalization.h"

int main(int argc, char **argv)
{
	ros::init(argc, argv, "vloc");

	ros::NodeHandle nh;

	//Create Nodes
	VLocalization vlocalization;

	//Process Jobs
	ros::spin();

	//Release Nodes
	return 0;
}
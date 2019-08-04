#include "ros/ros.h"

#include <tf/tf.h>
#include <tf/transform_listener.h>

int main(int argc, char **argv)
{
	ros::init(argc, argv, "testtf");

	ros::NodeHandle nh;

    ros::Duration sleeper(1 / 100);

    tf::TransformListener tf_listener;

	//Process Jobs
    int idx = 0;
	while(ros::ok()) {
        ros::Time tf_time = ros::Time::now();
        tf::StampedTransform tf_result;

        idx++;
        printf("Started tf %d\n", idx);
        try {
            tf_listener.waitForTransform("map", "tag_0", tf_time, ros::Duration(1));
            tf_listener.lookupTransform("map", "tag_0", tf_time, tf_result);
        }
        catch (tf::TransformException ex) {
            ROS_ERROR("%s",ex.what());
        }
        printf("OK tf\n");

        sleeper.sleep();
    }
	return 0;
}
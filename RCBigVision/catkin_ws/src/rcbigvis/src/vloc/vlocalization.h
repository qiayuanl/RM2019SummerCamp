#ifndef __VLOCALIZATION_H__
#define __VLOCALIZATION_H__

#include "ros/ros.h"
#include "vlocalization.h"
#include "apriltag_ros/AprilTagDetectionArray.h"
#include <tf/transform_listener.h>
#include <tf/transform_broadcaster.h>
#include <geometry_msgs/Twist.h>

class VLocalization {
public:
    VLocalization();

    //Update Functions
    void update();

    //Callback Functions
    void TagDetectionCallback(const apriltag_ros::AprilTagDetectionArray::ConstPtr& msg);
private:
    ros::Subscriber detect_sub;
    ros::Publisher  vloc_pub;
};

#endif
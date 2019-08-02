#include "vlocalization.h"

VLocalization::VLocalization() {
    ros::NodeHandle node_priv;

    //Setup Comm
    detect_sub = node_priv.subscribe<apriltag_ros::AprilTagDetectionArray> ("tag_detections", 100, &VLocalization::TagDetectionCallback, this);
    vloc_pub   = node_priv.advertise<geometry_msgs::Pose>                  ("vloc", 100);
}

void VLocalization::update() {

}

void VLocalization::TagDetectionCallback(const apriltag_ros::AprilTagDetectionArray::ConstPtr& msg) {

}
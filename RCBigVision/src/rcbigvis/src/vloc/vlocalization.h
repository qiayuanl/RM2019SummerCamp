#ifndef __VLOCALIZATION_H__
#define __VLOCALIZATION_H__

#include "ros/ros.h"
#include "vlocalization.h"
#include "apriltag_ros/AprilTagDetectionArray.h"

#include <tf/tf.h>
#include <tf/transform_listener.h>
#include <tf/transform_broadcaster.h>

#include <geometry_msgs/Twist.h>
#include <nav_msgs/Odometry.h>
#include <string>

template<typename T, int filter_size>
class MovingAverage {
private:
    T sum = 0;

    T buffer[filter_size];
    int pointer = 0;
    int size = 0;

public:
    MovingAverage() { memset(buffer, 0, sizeof(T) * filter_size); }
    T filter(T val) {
        //push
        sum = sum - buffer[pointer] + val;
        buffer[pointer] = val;
        pointer = (pointer + 1) % filter_size;

        if(size < filter_size) size++;

        return sum / size;
    }
};

class VLocalization {
public:
    VLocalization();

    void FuseDetectedTags(const apriltag_ros::AprilTagDetectionArray::ConstPtr& msg, const std::string camera_frame);

    //Callback Functions
    void TagDetection_0_Callback(const apriltag_ros::AprilTagDetectionArray::ConstPtr& msg);
    void TagDetection_1_Callback(const apriltag_ros::AprilTagDetectionArray::ConstPtr& msg);
private:
    MovingAverage<double,      3> Filter_X, Filter_Y, Filter_Yaw_X, Filter_Yaw_Y;

    ros::Subscriber detect_0_sub;
    ros::Subscriber detect_1_sub;
    ros::Publisher  vloc_pub;

    tf::TransformListener    tf_listener;
    tf::TransformBroadcaster tf_broadcaster;
};

#endif

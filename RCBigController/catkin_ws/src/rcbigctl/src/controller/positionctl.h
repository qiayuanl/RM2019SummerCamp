#ifndef __POSITIONCTL_H__
#define __POSITIONCTL_H__

#include <cmath>
#include <algorithm>

#include <ros/ros.h>
#include <tf/tf.h>

#include <geometry_msgs/Twist.h>
#include <geometry_msgs/Pose.h>
#include <nav_msgs/Odometry.h>
#include <std_msgs/Float64MultiArray.h>

#include <dynamic_reconfigure/server.h>
#include "rcbigctl/ControllerConfig.h"

class PositionCtl {
public:
    PositionCtl();
    ~PositionCtl();

    void update();
    void UpdateCloseloop();

    /* Callback Funcs */
    void CallbackDynamicParam( rcbigctl::ControllerConfig &config, uint32_t level );
    void CallbackPosition(     const nav_msgs::Odometry::ConstPtr& odom    );
    void CallbackSetpoint(     const geometry_msgs::Pose::ConstPtr& pose    );
private:
    /*
     * Handles
    */
    ros::Subscriber pos_sub;
    ros::Subscriber setpoint_sub;
    ros::Publisher	twist_pub;

    /*
     * Closeloop Paramters
     */
    double Kp_X;
    double Kp_Y;
    double Kp_W;

    /*
     * Position
     */
    double X, Y, W;

    /*
     * Setpoint
     */
    double Set_X, Set_Y, Set_W;
};

#endif
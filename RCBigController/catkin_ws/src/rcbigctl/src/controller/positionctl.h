#ifndef __POSITIONCTL_H__
#define __POSITIONCTL_H__

#include <cmath>
#include <algorithm>

#include <ros/ros.h>
#include <tf/tf.h>
#include <tf/transform_listener.h>

#include <geometry_msgs/Twist.h>
#include <geometry_msgs/Pose.h>
#include <nav_msgs/Odometry.h>
#include <std_msgs/Float64MultiArray.h>

#include <dynamic_reconfigure/server.h>
#include "rcbigctl/ControllerConfig.h"

struct RampFilter {
    ros::Time last_time = ros::Time(0);
    double last_value = 0;

    double limit_abs(double x, double max_abs) {
        double sign = (x < 0) ? -1 : 1;
        return sign * std::min( std::abs(x), std::abs(max_abs) );
    }
    double filter(double set_value, double max_value, double accel) {
        double dt = (ros::Time::now() - last_time).toSec();
        if(!dt) {
            return set_value;
        }

        //limit input
        set_value = limit_abs(set_value, max_value);

        //ramp filter
        last_time = ros::Time::now();
        last_value += limit_abs(set_value - last_value, accel * dt);
        return last_value;
    }
};

class PositionCtl {
public:
    PositionCtl();
    ~PositionCtl();

    void update();
    void UpdateCloseloop();

    /* Callback Funcs */
    void CallbackDynamicParam( rcbigctl::ControllerConfig &config, uint32_t level );
    void CallbackPosition(     const nav_msgs::Odometry::ConstPtr& odom    );
    void CallbackSetpoint(     const geometry_msgs::PoseStamped::ConstPtr& pose    );
private:
    /*
     * Handles
    */
    ros::Subscriber pos_sub;
    ros::Subscriber setpoint_sub;
    ros::Publisher	twist_pub;
    tf::TransformListener tf_pos;

    /*
     * Closeloop Paramters
     */
    double Kp_X, Max_X, A_X;
    double Kp_Y, Max_Y, A_Y;
    double Kp_W, Max_W, A_W;

    /*
     * Position
     */
    double X, Y, W;

    /*
     * Setpoint
     */
    double Set_X, Set_Y, Set_W;

    RampFilter FoutX, FoutY, FoutW;
};

#endif
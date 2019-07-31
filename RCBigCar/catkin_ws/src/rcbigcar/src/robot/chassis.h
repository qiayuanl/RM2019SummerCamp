#ifndef __CHASSIS_H__
#define __CHASSIS_H__

#include "motor.h"
#include "hardware.h"
#include "config.h"

#include <cmath>
#include <algorithm>

#include <ros/ros.h>
#include <tf/tf.h>

#include <geometry_msgs/Twist.h>
#include <nav_msgs/Odometry.h>
#include <std_msgs/Float64MultiArray.h>

#include <dynamic_reconfigure/server.h>
#include "openworld/ChassisConfig.h"

class Chassis {
public:
    Chassis();
    ~Chassis();

    /* Update Funcs */
    void update();
    void UpdateOdometry();
    void UpdateWatchdog();
    void UpdateDebug();

    /* Publish Funcs */
    void PublishOdometry();

    /* Callback Funcs */
    void CallbackDynamicParam( one_bot::ChassisConfig &config, uint32_t level );
    void CallbackVelocity(     const geometry_msgs::Twist::ConstPtr& twist    );
private:
    /*
     * Config
    */
    bool   Config_IsDebug;

    double Dyn_Config_MaxVel;

    /*
     * Dynamic Config
     */
    dynamic_reconfigure::Server<one_bot::ChassisConfig>               DynamicParamServer;
    dynamic_reconfigure::Server<one_bot::ChassisConfig>::CallbackType DynamicParamFunc;

    /*
     * Handles
    */
    ros::Publisher	odom_pub;
    ros::Subscriber twist_sub;

    /* 
    * Motor
    */
    Motor      *motors[4];
    ros::Time  motorWatchdog;

    /*
    * Odometry
    */
    double x, y, theta;
    double lastx, lasty, lasttheta;
    ros::Time lastt;

    double last_position[4];

    /*
    * Debug
    */
    ros::Publisher dbg_spd_setpoint_pub;
    ros::Publisher dbg_spd_real_pub;
};

#endif
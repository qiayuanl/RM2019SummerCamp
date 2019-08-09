#ifndef __MOTION_H__
#define __MOTION_H__

#include "motor.h"
#include "hardware.h"
#include "config.h"

#include <cmath>
#include <algorithm>

#include <ros/ros.h>
#include <tf/tf.h>

#include <std_msgs/Float64MultiArray.h>

#include <dynamic_reconfigure/server.h>
#include "rcbigcar/MotionConfig.h"

class Motion
{
  public:
    Motion();
    ~Motion();

    void update();
    void UpdateWatchdog();
    void UpdateMotors();
    void UpdatePublisher();

    void CallbackDynamicParam(rcbigcar::MotionConfig &config, uint32_t level);
    void CallbackSetpoint(const std_msgs::Float64MultiArray::ConstPtr &setpoint);
  private:
    /*
     * Handles
    */
    ros::Subscriber motor_sub;
    ros::Publisher status_pub;

    /*
     * Motors
     */
    Motor *motors[MOTION_MOTOR_COUNT];

    /*
     * Watchdog
     */
    ros::Time motionWatchdog;
};

#endif
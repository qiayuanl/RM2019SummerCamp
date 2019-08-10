#ifndef __CHASSIS_H__
#define __CHASSIS_H__

#include "config.h"
#include "hardware.h"
#include "motor.h"

#include <algorithm>
#include <cmath>

#include <ros/ros.h>
#include <tf/tf.h>
#include <tf/transform_broadcaster.h>
#include <tf/transform_listener.h>

#include <geometry_msgs/Twist.h>
#include <nav_msgs/Odometry.h>
#include <std_msgs/Float64MultiArray.h>

#include <dynamic_reconfigure/server.h>
#include "rcbigcar/ChassisConfig.h"

class Chassis
{
public:
  Chassis();
  ~Chassis();

  /* Internal Funcs */
  //double ReadGyroAngle();

  /* Update Funcs */
  void update();
  void UpdateOdometry();
  void UpdateWatchdog();
  void UpdateDebug();

  /* Publish Funcs */
  void PublishPosition();

  /* Callback Funcs */
  void CallbackDynamicParam(rcbigcar::ChassisConfig& config, uint32_t level);
  void CallbackVelocity(const geometry_msgs::Twist::ConstPtr& twist);
  //void CallbackVLocalization(const geometry_msgs::Pose::ConstPtr& pose);

private:
  /*
   * Config
  */
  bool Config_IsDebug;

  double Dyn_Config_MaxVel;
  //double Dyn_Config_VisualVel;
  //double Dyn_Config_TimeDelay;
  /*
   * Handles
   */
  //ros::Publisher pos_pub;
  //ros::Subscriber vloc_sub;
  ros::Subscriber twist_sub;
  tf::TransformBroadcaster tf_pos_pub;
  //tf::TransformListener tf_listener;

  /*
  * Motor
  */
  Motor* motors[4];
  ros::Time motorWatchdog;

  /*
  * Odometry
  */
  /* bool InitialPoseGot;
  double GyroCorrection;
  double AngularVelocity; */

  double x, y, theta;
  //double lastx, lasty, lasttheta;
  //ros::Time lastt;

  double last_position[4];

  /*
  * Debug
  */
  ros::Publisher dbg_spd_setpoint_pub;
  ros::Publisher dbg_spd_real_pub;
  //ros::Publisher dbg_pose_pub;
};

#endif
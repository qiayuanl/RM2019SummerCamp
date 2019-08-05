
#include <algorithm>
#include <cmath>

#include <ros/ros.h>
#include <tf/tf.h>
#include <tf/transform_listener.h>

#include <geometry_msgs/Pose.h>
#include <geometry_msgs/Twist.h>
#include <nav_msgs/Odometry.h>
#include <nav_msgs/Path.h>
#include <std_msgs/Float64MultiArray.h>

#include <dynamic_reconfigure/server.h>
#include "simple_controller/controllerConfig.h"

double yawFromQuaternion(const geometry_msgs::Quaternion& quat)
{
  tf::Quaternion q_orient(quat.x, quat.y, quat.z, quat.w);
  tf::Matrix3x3 m_orient(q_orient);

  double roll, pitch, yaw;
  m_orient.getRPY(roll, pitch, yaw);

  return yaw;
}

class controller
{
private:
  /*
   * Handles
  */
  ros::Subscriber pos_sub;
  ros::Subscriber setpoint_sub;
  ros::Subscriber path_sub;
  ros::Publisher twist_pub;
  tf::TransformListener tf_pos;

  /*
   * Closeloop Paramters
   */
  double max_linear_speed, max_angular_speed, max_angle_diff, kp;
  /*
   * Position
  */
  double x, y, w;

  /*
   * Setpoint
   */
  double set_x, set_y, set_w;

  void updateCloseloop();

  void setPose(const geometry_msgs::Pose& pose);

public:
  controller(/* args */);
  ~controller();
  /* Callback Funcs */
  void CallbackDynamicParam(simple_controller::controllerConfig& config, uint32_t level);
  void CallbackPosition(const nav_msgs::Odometry::ConstPtr& odom);
  void CallbackSetPose(const geometry_msgs::PoseStamped::ConstPtr& pose);
  void CallbackPath(const nav_msgs::Path::ConstPtr& path);
};

controller::controller(/* args */)
{
  ros::NodeHandle node_priv;

  // Setup controller paramters

  // Setup position paramters
  x = y = w = 0;
  set_x = set_y = set_w = 0;

  // Setup Reconfigurable Paramters
  static ros::NodeHandle DynamicParamNodeHandle("~/position");
  static dynamic_reconfigure::Server<simple_controller::controllerConfig> DynamicParamServer(DynamicParamNodeHandle);
  DynamicParamServer.setCallback(boost::bind(&controller::CallbackDynamicParam, this, _1, _2));
}

controller::~controller()
{
}

void controller::CallbackDynamicParam(simple_controller::controllerConfig& config, uint32_t level)
{
  kp = config.kp;
  max_linear_speed = config.max_linear_speed;
  max_angular_speed = config.max_angular_speed;

  max_angle_diff = config.max_angle_diff;
}
void controller::CallbackPosition(const nav_msgs::Odometry::ConstPtr& odom)
{
  x = odom->pose.pose.position.x;
  y = odom->pose.pose.position.y;
  w = fmod(yawFromQuaternion(odom->pose.pose.orientation), 2 * M_PI);

  updateCloseloop();
}

void controller::setPose(const geometry_msgs::Pose& pose)
{
  set_x = pose.position.x;
  set_y = pose.position.y;
  set_w = fmod(yawFromQuaternion(pose.orientation), 2 * M_PI);
}

void controller::CallbackSetPose(const geometry_msgs::PoseStamped::ConstPtr& pose)
{
  setPose(pose->pose);
}

void controller::CallbackPath(const nav_msgs::Path::ConstPtr& path)
{
  setPose(path->poses[0].pose);
}

void controller::updateCloseloop()
{
  geometry_msgs::Twist twist;
  twist.linear.z = 0;
  twist.angular.x = 0;
  twist.angular.y = 0;

  double diff_w = set_w - w;

  // calculate velocity

  if (diff_w > 0)
  {
    twist.angular.z = std::min(kp * diff_w, 2.0);
  }
  else
  {
    twist.angular.z = std::max(kp * diff_w, -2.0);
  }

  twist.linear.x = max_linear_speed * (1.0 - std::abs(diff_w) / (max_angle_diff));

  twist_pub.publish(twist);
}

int main(int argc, char** argv)
{
  ros::init(argc, argv, "controller");

  ros::NodeHandle nh;

  // Create Nodes
  ros::Rate loop_rate(100);

  // Process Jobs
  while (ros::ok())
  {
    ros::spinOnce();
    loop_rate.sleep();
  }

  // Release Nodes
  return 0;
}
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
#include "omnidir_controller/controllerConfig.h"

double yawFromQuaternion(const geometry_msgs::Quaternion& quat)
{
  tf::Quaternion q_orient(quat.x, quat.y, quat.z, quat.w);
  tf::Matrix3x3 m_orient(q_orient);

  double roll, pitch, yaw;
  m_orient.getRPY(roll, pitch, yaw);

  return yaw;
}
double limit_abs(double x, double max_abs)
{
  double sign = (x < 0) ? -1 : 1;
  return sign * std::min(std::abs(x), std::abs(max_abs));
}

double AngularMinus(double a, double b)
{
  a = fmod(a, 2.0 * M_PI);
  b = fmod(b, 2.0 * M_PI);

  double res1 = a - b;
  double res2 = (a < b) ? (a + 2 * M_PI - b) : (a - 2 * M_PI - b);

  return (std::abs(res1) < std::abs(res2)) ? res1 : res2;
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
  double max_linear_speed, max_angular_speed, max_angle_diff, goal_tolerance, kw, kx, ky;
  /*
   * Position
  */
  double x, y, w;

  /*
   * Setpoint
   */
  double set_x, set_y, set_w;
  bool near_goal;

  void updateCloseloop();

  void setPose(const geometry_msgs::Pose& pose);

public:
  controller(/* args */);
  ~controller();
  /* Callback Funcs */
  void CallbackDynamicParam(omnidir_controller::controllerConfig& config, uint32_t level);
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
  static dynamic_reconfigure::Server<omnidir_controller::controllerConfig> DynamicParamServer(DynamicParamNodeHandle);
  DynamicParamServer.setCallback(boost::bind(&controller::CallbackDynamicParam, this, _1, _2));

  // Setup Comm
  pos_sub = node_priv.subscribe<nav_msgs::Odometry>("odom", 100, &controller::CallbackPosition, this);
  setpoint_sub =
      node_priv.subscribe<geometry_msgs::PoseStamped>("move_base_simple/goal", 100, &controller::CallbackSetPose, this);
  path_sub = node_priv.subscribe<nav_msgs::Path>("local_planner/path", 100, &controller::CallbackPath, this);
  twist_pub = node_priv.advertise<geometry_msgs::Twist>("velocity", 100);
}

controller::~controller()
{
}

void controller::CallbackDynamicParam(omnidir_controller::controllerConfig& config, uint32_t level)
{
  kx = config.kx;
  ky = config.ky;
  kw = config.kw;
  max_linear_speed = config.max_linear_speed;
  max_angular_speed = config.max_angular_speed;
  max_angle_diff = config.max_angle_diff;
  goal_tolerance = config.goal_tolerance;
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
  int poses_length = path->poses.size();
  if (poses_length >= goal_tolerance)
  {
    near_goal = false;
    setPose(path->poses[0].pose);
  }
  else
  {
    near_goal = true;
    setPose(path->poses[poses_length - 1].pose);
  }
}

void controller::updateCloseloop()
{
  geometry_msgs::Twist twist;
  geometry_msgs::Vector3Stamped v_base;
  geometry_msgs::Vector3Stamped v_map;

  twist.linear.z = 0;
  twist.angular.x = 0;
  twist.angular.y = 0;

  double diff_w = AngularMinus(set_w, w);
  double diff_xy = sqrt((set_x - x) * (set_x - x) + (set_y - y) * (set_y - y));
  // calculate velocity

  if (!near_goal)
  {
    v_map.vector.x = cos(set_w) * max_linear_speed;
    v_map.vector.y = sin(set_w) * max_linear_speed;
    v_map.vector.z = 0;
    twist.angular.z = 0;
  }
  else
  {
    v_map.header.stamp = ros::Time();
    v_map.header.frame_id = "map";

    v_map.vector.x = kx * (set_x - x);
    v_map.vector.y = ky * (set_y - y);
    v_map.vector.z = 0;
    if (diff_w > 0)
    {
      twist.angular.z = std::min(kw * diff_w, max_angular_speed);
    }
    else
    {
      twist.angular.z = std::max(kw * diff_w, -max_angular_speed);
    }
  }
  // Transform map Velocity to robot base Velocity
  try
  {
    tf_pos.transformVector("base_fused", v_map, v_base);
  }
  catch (tf::TransformException ex)
  {
    ROS_ERROR("%s", ex.what());
  }
  twist.linear.x = limit_abs(v_base.vector.x, max_linear_speed);
  twist.linear.y = limit_abs(v_base.vector.y, max_linear_speed);

  twist_pub.publish(twist);
}

int main(int argc, char** argv)
{
  ros::init(argc, argv, "controller");

  ros::NodeHandle nh;
  controller omnidirctl;
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
#include "ros/ros.h"
#include <tf/transform_listener.h>
#include <tf/transform_broadcaster.h>
#include <geometry_msgs/Twist.h>
#include <cmath>
double x, y, theta;
double r = 0.8;
double w = 45;

double t;

geometry_msgs::PoseStamped plan;
tf::Quaternion plan_q;
int main(int argc, char **argv) {
  ros::init(argc, argv, "base_update_node");

  ros::NodeHandle nh;
  ros::Publisher planner_pub = nh.advertise<geometry_msgs::PoseStamped>("move_base_simple/goal", 100);
  ros::Rate rate(100);
  while (ros::ok()) {
    x = cos(0.01745 * w * t) * r;
    y = sin(0.01745 * w * t) * r;
    theta = 0.01745*(w * t) + 3.1415926;
    plan_q.setRPY(0, 0, theta);
    plan.header.stamp = ros::Time::now();
    plan.header.frame_id = "/map";
    plan.pose.orientation.x = plan_q.getX();
    plan.pose.orientation.y = plan_q.getY();
    plan.pose.orientation.z = plan_q.getZ();
    plan.pose.orientation.w = plan_q.getW();
    plan.pose.position.x = x;
    plan.pose.position.y = y;
    planner_pub.publish(plan);
    t += 0.01;
    rate.sleep();
  }
  return 0;
}

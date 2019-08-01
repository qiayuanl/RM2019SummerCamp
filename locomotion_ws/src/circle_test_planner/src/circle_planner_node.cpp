#include "ros/ros.h"
#include <tf/transform_listener.h>
#include <tf/transform_broadcaster.h>
#include <geometry_msgs/Twist.h>
#include <cmath>
double x, y, theta;
double r = 0.5;
double w = 180;

double t;

geometry_msgs::Pose plan;
tf::Quaternion plan_q;
int main(int argc, char **argv) {
  ros::init(argc, argv, "base_update_node");

  ros::NodeHandle nh;
  ros::Publisher planner_pub = nh.advertise<geometry_msgs::Pose>("setpose", 1000);
  ros::Rate rate(100);
  while (ros::ok()) {
    x = cos(0.01745 * w * t) * r;
    y = sin(0.01745 * w * t) * r;
    theta = w * t + 3.1415;
    plan_q.setRPY(0, 0, theta);
    plan.orientation.x = plan_q.getX();
    plan.orientation.y = plan_q.getY();
    plan.orientation.z = plan_q.getZ();
    plan.orientation.w = plan_q.getW();
    plan.position.x = x;
    plan.position.y = y;
    planner_pub.publish(plan);
    t += 0.01;
    rate.sleep();
  }
  return 0;
}

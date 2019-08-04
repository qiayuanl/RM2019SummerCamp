/****************************************************************************
 *  Copyright (C) 2019 RoboMaster.
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of 
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program. If not, see <http://www.gnu.org/licenses/>.
 ***************************************************************************/

#include <tf/tf.h>
#include <tf/transform_listener.h>

#include <nav_msgs/Path.h>
#include <geometry_msgs/PoseWithCovarianceStamped.h>
#include <visualization_msgs/Marker.h>

#include <Eigen/Eigen>
#include <chrono>

#include "utility.h"

namespace robomaster{
class GlobalPlanner{
 public:
  GlobalPlanner(ros::NodeHandle& given_nh):
      nh(given_nh), record_(false){

    nh.param<int>("point_num", point_num_, 10);
    nh.param<int>("plan_frequency", plan_freq_, 50);
    nh.param<std::string>("global_frame", global_frame_, "map");
    nh.param<double>("waypoint_distance", waypoint_dist_, 0.4);

    // -------------------visulize endpoints and trajectory---------------------
    tf_listener_ = std::make_shared<tf::TransformListener>();

    setpoint_pub_ = nh.advertise<visualization_msgs::Marker>("set_point", 10);
    global_path_pub_ = nh.advertise<nav_msgs::Path>("path", 10);

    // 2D Nav Goal to set point
    waypoint_sub_ = nh.subscribe("/clicked_point", 5, &GlobalPlanner::WaypointCallback,this);
    record_sub_ = nh.subscribe("/initialpose",1, &GlobalPlanner::RecordCallback,this);

//    plan_timer_ = nh.createTimer(ros::Duration(1.0/plan_freq_),&GlobalPlanner::Plan,this);

    record_timer_ = nh.createTimer(ros::Duration(0.02),&GlobalPlanner::Record,this);
    record_timer_.stop();

    path_.header.frame_id = global_frame_;


  }
  ~GlobalPlanner()= default;

 private:

//  void Plan(const ros::TimerEvent& event){
//  global_path_pub_.publish(path_);
//  }

  void Record(const ros::TimerEvent& event){

    geometry_msgs::PoseStamped tmp_pose;
    GetGlobalRobotPose(tf_listener_, global_frame_, tmp_pose);
    if(path_.poses.empty()){
      path_.poses.push_back(tmp_pose);
      ROS_INFO(" Record Point: 1");
    }
    else if(hypot(tmp_pose.pose.position.x - path_.poses.back().pose.position.x,
                  tmp_pose.pose.position.y - path_.poses.back().pose.position.y) > waypoint_dist_){
      path_.poses.push_back(tmp_pose);
      VisualizeSetPoints(path_);
      ROS_INFO(" Record Point: %d", static_cast<int>(path_.poses.size()));
    }

  }

  void VisualizeSetPoints(const nav_msgs::Path& path)
  {
    // send them to rviz
    for(int i = 0; i < path.poses.size(); ++i)
    {
      visualization_msgs::Marker p;
      p.header.frame_id = global_frame_;
      p.header.stamp = ros::Time::now();
      p.id = i;

      p.type = visualization_msgs::Marker::SPHERE;
      p.action = visualization_msgs::Marker::ADD;

      p.pose.position.x = path.poses[i].pose.position.x;
      p.pose.position.y = path.poses[i].pose.position.y;
      p.pose.position.z = path.poses[i].pose.position.z;
      p.pose.orientation.w = 1;
      p.pose.orientation.x = 0;
      p.pose.orientation.y = 0;
      p.pose.orientation.z = 0;

      p.scale.x = p.scale.y = p.scale.z = 0.2;

      p.color.a = p.color.r = 1.0;
      p.color.g = p.color.b = 0.0;

      p.lifetime = ros::Duration(20.0);

      setpoint_pub_.publish(p);
      ros::Duration(0.001).sleep();

    }
  }

  void WaypointCallback(const geometry_msgs::PointStamped::ConstPtr &msg)
  {
    if(record_){
      ROS_INFO("Currently Recording");
      return;
    }

    if(path_.poses.size() >= point_num_){
      path_.poses.clear();
    }

    geometry_msgs::PoseStamped pose;
    pose.header.frame_id = msg->header.frame_id;
    pose.pose.position = msg->point;
    pose.pose.position.z = 0;
    pose.pose.orientation.w = 1;
    if (pose.header.frame_id != global_frame_){
      TransformPose(tf_listener_, global_frame_, pose, pose);
    }

    path_.poses.push_back(pose);
    VisualizeSetPoints(path_);

    ROS_INFO("New waypoint added!");

    if(path_.poses.size() == point_num_)
    {
      ROS_INFO("Waypoint enough!");
      global_path_pub_.publish(path_);
    }
    else
    {
      ROS_INFO("%d more waypoints required", static_cast<int>(point_num_ - path_.poses.size()));
    }
  }

  void RecordCallback(const geometry_msgs::PoseWithCovarianceStamped::ConstPtr &msg){

    if(record_){
      record_ = false;
      ROS_INFO("Record complete");
      ROS_ASSERT(record_timer_.hasStarted());
      record_timer_.stop();
      global_path_pub_.publish(path_);

    } else {
      record_ = true;
      ROS_INFO("Start to record, stop planning");
      path_.poses.clear();
      ROS_ASSERT(!record_timer_.hasStarted());
      record_timer_.start();
    }
  }
 private:
  ros::NodeHandle nh;
  std::shared_ptr<tf::TransformListener> tf_listener_;
  std::string global_frame_;
//  ros::Timer plan_timer_;

  ros::Publisher global_path_pub_;
  ros::Publisher setpoint_pub_;

  ros::Subscriber waypoint_sub_;
  ros::Subscriber record_sub_;

  ros::Timer record_timer_;

  nav_msgs::Path path_;
  bool record_;
  int point_num_;
  double waypoint_dist_;
  int plan_freq_;

};
}

using namespace robomaster;
int main(int argc, char **argv)
{
  ros::init(argc, argv, "global_planner");
  ros::NodeHandle nh("~");
  GlobalPlanner global_planner(nh);
  ros::spin();
  return 0;
}



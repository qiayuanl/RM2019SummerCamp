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
#include <nav_msgs/Odometry.h>
#include <geometry_msgs/Twist.h>

#include <Eigen/Eigen>
#include <chrono>

#include "cubic_spline/cubic_spline_ros.h"
#include "utility.h"


namespace robomaster{


class LocalPlanner{
 public:
  LocalPlanner(ros::NodeHandle& given_nh):nh(given_nh),plan_(false), prune_index_(0){


    nh.param<double>("max_speed", max_speed_, 2.0);
    double max_angle_diff;
    nh.param<double>("max_angle_diff", max_angle_diff, 60);
    max_angle_diff_ = max_angle_diff * M_PI / 180;
    nh.param<double>("p_coeff", p_coeff_, 10.0);
    nh.param<int>("plan_frequency", plan_freq_, 50);
    nh.param<double>("goal_tolerance", goal_tolerance_, 0.1);
    nh.param<double>("prune_ahead_distance", prune_ahead_dist_, 0.3);
    nh.param<std::string>("global_frame", global_frame_, "odom");

    tf_listener_ = std::make_shared<tf::TransformListener>();

    local_path_pub_= nh.advertise<nav_msgs::Path>("path", 5);

    cmd_vel_pub_ = nh.advertise<geometry_msgs::Twist>("/cmd_vel", 1);

    global_path_sub_ = nh.subscribe("/global_planner/path", 5, &LocalPlanner::GlobalPathCallback,this);

    plan_timer_ = nh.createTimer(ros::Duration(1.0/plan_freq_),&LocalPlanner::Plan,this);



  }
  ~LocalPlanner()= default;
  void GlobalPathCallback(const nav_msgs::PathConstPtr & msg){
    if (!msg->poses.empty()){
      global_path_ = *msg;
      prune_index_ = 0;
      plan_ = true;
    }
  }
 private:

  void Plan(const ros::TimerEvent& event){

    if (plan_){

      auto begin = std::chrono::steady_clock::now();

      // 1. Update the transform from global path frame to local planner frame
      UpdateTransform(tf_listener_, global_frame_,
                      global_path_.header.frame_id, global_path_.header.stamp,
                      global2path_transform_);//source_time needs decided


      // 2. Get current robot pose in global path frame
      geometry_msgs::PoseStamped robot_pose;
      GetGlobalRobotPose(tf_listener_, global_path_.header.frame_id, robot_pose);

      // 3. Check if robot has already arrived with given distance tolerance
      if (GetEuclideanDistance(robot_pose,global_path_.poses.back())<= goal_tolerance_
          && prune_index_ == global_path_.poses.size() - 1){
        plan_ = false;
        geometry_msgs::Twist cmd_vel;
        cmd_vel.linear.x = 0;
        cmd_vel.linear.y = 0;
        cmd_vel.angular.z = 0;
        cmd_vel_pub_.publish(cmd_vel);
        ROS_INFO("Planning Success!");
        return;
      }

      // 4. Get prune index from given global path
      FindNearstPose(robot_pose, global_path_, prune_index_, prune_ahead_dist_);// TODO: double direct prune index is needed later!

      // 5. Generate the prune path and transform it into local planner frame
      nav_msgs::Path prune_path, local_path;

      local_path.header.frame_id = global_frame_;
      prune_path.header.frame_id = global_frame_;

      geometry_msgs::PoseStamped tmp_pose;
      tmp_pose.header.frame_id = global_frame_;

      TransformPose(global2path_transform_, robot_pose, tmp_pose);
      prune_path.poses.push_back(tmp_pose);

      for (int i = prune_index_; i < global_path_.poses.size(); i++){
        TransformPose(global2path_transform_, global_path_.poses[i], tmp_pose);
        prune_path.poses.push_back(tmp_pose);

      }

      // 6. Generate the cubic spline trajectory from above prune path
      GenTraj(prune_path, local_path);
      local_path_pub_.publish(local_path);

      // 7. Follow the trajectory and calculate the velocity
      geometry_msgs::Twist cmd_vel;
      FollowTraj(prune_path.poses.front(), local_path, cmd_vel);
      cmd_vel_pub_.publish(cmd_vel);

      auto plan_time = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now() - begin);
      ROS_INFO("Planning takes %f ms and passed %d/%d.",
               plan_time.count()/1000.,
               prune_index_,
               static_cast<int>(global_path_.poses.size()));
    }

  }

  void FollowTraj(const geometry_msgs::PoseStamped& robot_pose,
                  const nav_msgs::Path& traj,
                  geometry_msgs::Twist& cmd_vel){
    double diff_yaw = GetYawFromOrientation(traj.poses[0].pose.orientation)
        - GetYawFromOrientation(robot_pose.pose.orientation);

    // set it from -PI to PI
    if(diff_yaw > M_PI){
      diff_yaw -= 2*M_PI;
    } else if(diff_yaw < -M_PI){
      diff_yaw += 2*M_PI;
    }

    // calculate velocity

    if( diff_yaw > 0 ){
      cmd_vel.angular.z = std::min(p_coeff_*diff_yaw, 2.0);
    }else{
      cmd_vel.angular.z = std::max(p_coeff_*diff_yaw, -2.0);
    }


    cmd_vel.linear.x = max_speed_*(1.0-std::abs(diff_yaw)/(max_angle_diff_));
    cmd_vel.linear.y = 0;
    if(std::abs(diff_yaw) > max_angle_diff_){
      cmd_vel.linear.x = 0;
    }
  }

 private:

  void FindNearstPose(geometry_msgs::PoseStamped& robot_pose,nav_msgs::Path& path, int& prune_index, double prune_ahead_dist = 0.3){

    double dist_threshold = 10;// threshold is 10 meters (basically never over 10m i suppose)
    double sq_dist_threshold = dist_threshold * dist_threshold;
    double sq_dist;
    if(prune_index!=0){
      sq_dist = GetEuclideanDistance(robot_pose,path.poses[prune_index-1]);
    }else{
      sq_dist = 1e10;
    }

    double new_sq_dist = 0;
    while (prune_index < (int)path.poses.size()) {
      new_sq_dist = GetEuclideanDistance(robot_pose,path.poses[prune_index]);
      if (new_sq_dist > sq_dist && sq_dist < sq_dist_threshold) {

        //Judge if it is in the same direction and sq_dist is further than 0.3 meters
        if ((path.poses[prune_index].pose.position.x - robot_pose.pose.position.x) *
            (path.poses[prune_index-1].pose.position.x - robot_pose.pose.position.x) +
            (path.poses[prune_index].pose.position.y - robot_pose.pose.position.y) *
                (path.poses[prune_index-1].pose.position.y - robot_pose.pose.position.y) > 0
            && sq_dist > prune_ahead_dist) {
          prune_index--;
        }else{
          sq_dist = new_sq_dist;
        }

        break;
      }
      sq_dist = new_sq_dist;
      ++prune_index;
    }

    prune_index = std::min(prune_index, (int)(path.poses.size()-1));

  }

  ros::NodeHandle nh;
  std::shared_ptr<tf::TransformListener> tf_listener_;
  tf::StampedTransform global2path_transform_;

  std::string global_frame_;
  ros::Timer plan_timer_;

  ros::Subscriber global_path_sub_;
  ros::Publisher local_path_pub_;
  ros::Publisher cmd_vel_pub_;

  bool plan_;
  int prune_index_;
  nav_msgs::Path global_path_;

  double max_speed_;
  double max_angle_diff_;
  double p_coeff_;
  double goal_tolerance_;
  double prune_ahead_dist_;
  int plan_freq_;

};
}

using namespace robomaster;
int main(int argc, char **argv)
{
  ros::init(argc, argv, "local_planner");
  ros::NodeHandle nh("~");
  LocalPlanner local_planner(nh);
  ros::spin();
  return 0;
}



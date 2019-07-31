#include "ros/ros.h"
#include "apriltag_ros/AprilTagDetectionArray.h"
#include <tf/transform_listener.h>
#include <tf/transform_broadcaster.h>
#include <nav_msgs/Odometry.h>
#include <memory>
#include <eigen3/Eigen/Dense>
#include <eigen3/Eigen/Geometry>
#include <cmath>

tf::Transform odom_transform;
tf::Quaternion odom_q;
tf::TransformListener* listener;
tf::Vector3 odom_v_old(0.0,0.0,0.0);
int not_seeing_tag_counter = 0;
bool init = false;
//// Nomenclature: 1. parentFrame_childFrame (If the parentFrame is map, then the parent frame is neglected.)
////               2. _r 3x3 tf rotation matrix, _v 1x3 tf vector, _q tf quaternion.

void aprilCallback(const apriltag_ros::AprilTagDetectionArray::ConstPtr& msg)
{

    static tf::TransformBroadcaster br;
    tf::StampedTransform april_transform;
    tf::StampedTransform odom_cam_transform;
    tf::StampedTransform map_tag_transform;
    double odom_cam_x; double odom_cam_y;
    double tag_cam_x =0; double tag_cam_y =0; double tag_cam_z =0;
    double tag_x =0; double tag_y =0;
    tf::Quaternion odom_cam_q; tf::Quaternion tag_cam_q;
    int nearest_tag_id = 0;
    int dist_tag= 10000;



    if (!msg->detections.empty()){
        /// Obtain position information from AprilTag detection

        for (int i = 0; i < msg->detections.size(); ++i){
            if (((msg->detections[i].pose.pose.pose.position.x)*(msg->detections[i].pose.pose.pose.position.x)+
                    (msg->detections[i].pose.pose.pose.position.y)*(msg->detections[i].pose.pose.pose.position.y))<dist_tag){
                nearest_tag_id = i;
            }
        }
        //ROS_INFO_STREAM("Tag number "<< msg->detections[nearest_tag_id].id[0]);
        tag_cam_x = msg->detections[nearest_tag_id].pose.pose.pose.position.x;
        tag_cam_y = msg->detections[nearest_tag_id].pose.pose.pose.position.y;
        tag_cam_z = msg->detections[nearest_tag_id].pose.pose.pose.position.z;
        tag_cam_q.setX(msg->detections[nearest_tag_id].pose.pose.pose.orientation.x);
        tag_cam_q.setY(msg->detections[nearest_tag_id].pose.pose.pose.orientation.y);
        tag_cam_q.setZ(msg->detections[nearest_tag_id].pose.pose.pose.orientation.z);
        tag_cam_q.setW(msg->detections[nearest_tag_id].pose.pose.pose.orientation.w);
        tag_cam_q.normalize();

        // Obtain information from odometer to usb_cam
        try{
            listener->lookupTransform("odom", "usb_cam", ros::Time(0), odom_cam_transform);
        }

        catch (tf::TransformException ex){
            ROS_ERROR("1:%s",ex.what());
//            ros::Duration(1.0).sleep();
        }
        odom_cam_x = odom_cam_transform.getOrigin().x();
        odom_cam_y = odom_cam_transform.getOrigin().y();
        odom_cam_q = odom_cam_transform.getRotation();

        /// Obtain information from map to tag
        std::stringstream ss;
        ss << msg->detections[0].id[0];
        std::string tag_id = "tag_"+ss.str();
        ss.str("");

        try{
            listener->lookupTransform("map", tag_id , ros::Time(0), map_tag_transform);
        }

        catch (tf::TransformException ex){
            ROS_ERROR("2:%s",ex.what());
//            ros::Duration(1.0).sleep();
        }

        tag_x = map_tag_transform.getOrigin().x();
        tag_y = map_tag_transform.getOrigin().y();


        /// Calculations to find the transformation from map to odometer reference
        /// 1. Convert all the coordinate systems to the map coordinate frame
        /// 2. calculate /odom's pose relative to the map coordinate system

        tf::Matrix3x3 tag_cam_r(tag_cam_q);
        tf::Matrix3x3 odom_cam_r(odom_cam_q);
        tf::Matrix3x3 odom_r = tag_cam_r*(odom_cam_r.transpose());

        double cam_x = tag_cam_x + tag_x;
        double cam_y = tag_cam_y + tag_y;

        tf::Vector3 cam_v(cam_x, cam_y, tag_cam_z);
        tf::Vector3 odom_cam_v(odom_cam_x,odom_cam_y,tag_cam_z);
        tf::Vector3 odom_v = cam_v-odom_r*odom_cam_v;

        double odom_roll,odom_pitch,odom_yaw;
        odom_r.getRPY(odom_roll,odom_pitch,odom_yaw);
        odom_r.setRPY(0,0,odom_yaw);
        odom_v.setZ(0);

        /// To prevent odom outlier
        if (init && fabs((odom_v_old[0]*odom_v_old[0] +odom_v_old[1]*odom_v_old[1])-(odom_v[0]*odom_v[0] +odom_v[1]*odom_v[1]))> 0.4 && not_seeing_tag_counter <200){
            //ROS_INFO_STREAM("Abnormal distance "<< fabs((odom_v_old[0]*odom_v_old[0] +odom_v_old[1]*odom_v_old[1])-(odom_v[0]*odom_v[0] +odom_v[1]*odom_v[1])));
            //odom_v_old = odom_v;
            //not_seeing_tag_counter+=1;
            br.sendTransform(tf::StampedTransform(odom_transform, ros::Time::now(), "map", "odom"));
            return;
        }


        init = true;
        odom_r.getRotation(odom_q);
        odom_transform.setRotation(odom_q);
        odom_transform.setOrigin(odom_v);
        odom_v_old = odom_v;
        br.sendTransform(tf::StampedTransform(odom_transform, ros::Time::now(), "map", "odom"));
        not_seeing_tag_counter = 0;

    }else{
        not_seeing_tag_counter+=1;
        br.sendTransform(tf::StampedTransform(odom_transform, ros::Time::now(), "map", "odom"));
    }

}


int main(int argc, char **argv)
{
  ros::init(argc, argv, "odom_update_node");
  ros::NodeHandle nh;
  listener = new tf::TransformListener(ros::Duration(1));
  ros::Subscriber detection = nh.subscribe("tag_detections", 1000, &aprilCallback);

  ros::spin();
  return 0;
}

#include "ros/ros.h"
#include "apriltag_ros/AprilTagDetectionArray.h"
#include <tf/transform_listener.h>
#include <tf/transform_broadcaster.h>
#include <memory>
#include <geometry_msgs/Twist.h>
#include <cmath>

tf::Transform tag_base_transform;
tf::Quaternion tag_base_q;
tf::TransformListener* listener;
tf::Vector3 base_v_old(0.0,0.0,0.0);
geometry_msgs::Pose odom;
//// Nomenclature: 1. parentFrame_childFrame (If the parentFrame is map, then the parent frame is neglected.)
////               2. _r 3x3 tf rotation matrix, _v 1x3 tf vector, _q tf quaternion.
bool new_detection;

void aprilCallback(const apriltag_ros::AprilTagDetectionArray::ConstPtr& msg)
{

    static tf::TransformBroadcaster br;
    tf::StampedTransform base_cam_transform;

    double base_cam_x, base_cam_y, base_cam_z;
    double tag_cam_x =0; double tag_cam_y =0; double tag_cam_z =0;
    double tag_x =0; double tag_y =0;
    tf::Quaternion base_cam_q; tf::Quaternion tag_cam_q;
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
        try{
            listener->lookupTransform("base", "cam", ros::Time(0), base_cam_transform);
        }

        catch (tf::TransformException ex){
            ROS_ERROR("1:%s",ex.what());
//            ros::Duration(1.0).sleep();
        }
        base_cam_x = base_cam_transform.getOrigin().x();
        base_cam_y = base_cam_transform.getOrigin().y();
        base_cam_z = base_cam_transform.getOrigin().z();
        base_cam_q = base_cam_transform.getRotation();
        tf::Matrix3x3 tag_cam_r(tag_cam_q);
        tf::Matrix3x3 base_cam_r(base_cam_q);
        tf::Matrix3x3 tag_base_r = tag_cam_r*(base_cam_r.transpose());

        tf::Vector3 tag_cam_v(tag_cam_x, tag_cam_y, tag_cam_z);
        tf::Vector3 base_cam_v(base_cam_x,base_cam_y,base_cam_z);
        tf::Vector3 tag_base_v = tag_cam_v-tag_base_r*base_cam_v;
        double base_roll,base_pitch,base_yaw;

        tag_base_r.getRotation(tag_base_q);
        tag_base_transform.setRotation(tag_base_q);
        tag_base_transform.setOrigin(tag_base_v);
        br.sendTransform(tf::StampedTransform(tag_base_transform, ros::Time::now(), "tag_0", "base"));
        odom.position.x = tag_base_v.getX();
        odom.position.y = tag_base_v.getY();
        odom.orientation.x = tag_base_q.getX();
        odom.orientation.y = tag_base_q.getY();
        odom.orientation.z = tag_base_q.getZ();
        odom.orientation.w = tag_base_q.getW();
        new_detection = true;
    }
}

int main(int argc, char **argv)
{
  ros::init(argc, argv, "base_update_node");

  ros::NodeHandle nh;

  listener = new tf::TransformListener(ros::Duration(1));
  ros::Subscriber detection = nh.subscribe("tag_detections", 1000, &aprilCallback);
  ros::Publisher base_pub = nh.advertise<geometry_msgs::Pose>("vloc", 1000);
  ros::Rate rate(30);
  while (ros :: ok()) {
    if(new_detection)
    {
        new_detection = false;
        base_pub.publish(odom);
    }
    ros::spinOnce();
    rate.sleep();
  }
  return 0;
}

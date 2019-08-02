#include "vlocalization.h"
#include "lut.h"

double YawFromQuaternion(const tf::Quaternion &quat) {
	tf::Matrix3x3  m_orient(quat);

    double roll, pitch, yaw;
    m_orient.getRPY(roll, pitch, yaw);

    return yaw;
}

VLocalization::VLocalization() {
    ros::NodeHandle node_priv;

    //Setup Comm
    detect_sub = node_priv.subscribe<apriltag_ros::AprilTagDetectionArray> ("tag_detections", 100, &VLocalization::TagDetectionCallback, this);
    vloc_pub   = node_priv.advertise<geometry_msgs::Pose>                  ("vloc", 100);
}

void VLocalization::update() {

}

void VLocalization::TagDetectionCallback(const apriltag_ros::AprilTagDetectionArray::ConstPtr& msg) {
    //If no detection
    if (msg->detections.empty()) return;

    double Loc_X = 0;
    double Loc_Y = 0;
    double Loc_Yaw = 0;
    double Loc_Weight_Sum = 0;

    //Lookup Base -> Cam Transform
    tf::StampedTransform tf_base_cam;
    try{
        tf_listener.lookupTransform("base_link", "cam", ros::Time(0), tf_base_cam);
    }
    catch (tf::TransformException ex) {
        ROS_ERROR("%s",ex.what());
    }

    //Loop all detections
    for(int i = 0; i < msg->detections.size(); i++) {
        int ID                     = msg->detections[i].ID[0];
        tf::Vector3 tag_cam_v      = msg->detections[i].pose.pose.pose.position;
        tf::Quaternion tag_cam_q   = msg->detections[i].pose.pose.pose.orientation;
        tag_cam_q.normalize();

        //Transform Coordinate
        tf::Vector3   base_cam_v = tf_base_cam.getOrigin();
        tf::Matrix3x3 base_cam_r(tf_base_cam.getRotation());

        tf::Matrix3x3 tag_cam_r(tag_cam_q);

        tf::Matrix3x3 tag_base_r = tag_cam_r*(base_cam_r.transpose());
        tf::Vector3   tag_base_v = tag_cam_v-tag_base_r*base_cam_v;

        //Send TF
        tf::Quaternion tag_base_q;
        tf::Transform  tag_base_transform;
        tag_base_r.getRotation(tag_base_q);
        tag_base_transform.setRotation(tag_base_q);
        tag_base_transform.setOrigin(tag_base_v);
        tf_broadcaster.sendTransform(tf::StampedTransform(tag_base_transform, ros::Time::now(), "tag_" + std::to_string(ID), "base_vloc_tag_" + std::to_string(ID)));

        //Lookup TF
        tf::StampedTransform tf_world_base;
        try {
            tf_listener.lookupTransform("world", "base_vloc_tag_" + std::to_string(ID), ros::Time(0), tf_world_base);
        }
        catch (tf::TransformException ex) {
            ROS_ERROR("%s",ex.what());
        }

        double Cur_Base_X =   tf_world_base.getOrigin().getX();
        double Cur_Base_Y =   tf_world_base.getOrigin().getY();
        double Cur_Base_Yaw = YawFromQuaternion(tf_world_base.getRotation());

        //Calc Weight
        double Cur_Weight = 1 / tag_cam_v.length();

        //Sum Up Position Estimate
        Loc_X +=   Cur_Weight * Cur_Base_X;
        Loc_Y +=   Cur_Weight * Cur_Base_Y;
        Loc_Yaw += Cur_Weight * Cur_Base_Yaw;
        Loc_Weight_Sum += Cur_Weight;
    }

    if(!Loc_Weight_Sum) return;

    //Broadcast Pose
    geometry_msgs::Pose pose;

    pose.position.x = Loc_X / Loc_Weight_Sum;
    pose.position.y = Loc_Y / Loc_Weight_Sum;
    pose.position.z = 0;

    pose.orientation = tf::createQuaternionFromYaw(Loc_Yaw / Loc_Weight_Sum);

    vloc_pub.publish(pose);
}
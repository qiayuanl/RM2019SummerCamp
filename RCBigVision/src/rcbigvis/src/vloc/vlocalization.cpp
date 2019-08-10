#include "vlocalization.h"

const int TAG_COUNT = 7 * 9;

double YawFromQuaternion(const tf::Quaternion &quat) {
	tf::Matrix3x3  m_orient(quat);

    double roll, pitch, yaw;
    m_orient.getRPY(roll, pitch, yaw);

    return yaw;
}

inline std::string to_string(int x) {
    std::stringstream ss;
    ss << x;
    return ss.str();
}

VLocalization::VLocalization() {
    ros::NodeHandle node_priv;

    //Setup Comm
    detect_front_sub = node_priv.subscribe<apriltag_ros::AprilTagDetectionArray> ("cam_front/tag_detections", 100, &VLocalization::TagDetection_Front_Callback, this);
    detect_back_sub  = node_priv.subscribe<apriltag_ros::AprilTagDetectionArray> ("cam_back/tag_detections",  100, &VLocalization::TagDetection_Back_Callback, this);

    //Load tag pos
    TagTransforms.resize(TAG_COUNT);
    for(int id = 0; id < TAG_COUNT; id++) {
        //Lookup TF
        tf::StampedTransform tf_map_tag;
        try {
            tf_listener.waitForTransform("map", "tag_" + to_string(id), ros::Time(0), ros::Duration(1));
            tf_listener.lookupTransform ("map", "tag_" + to_string(id), ros::Time(0), tf_map_tag);

            TagTransforms[id] = tf_map_tag;
        }
        catch (tf::TransformException ex) {
            ROS_ERROR("%s",ex.what());
        }
    }
}

void VLocalization::FuseDetectedTags(const apriltag_ros::AprilTagDetectionArray::ConstPtr& msg, const std::string camera_frame) {
    //If no detection
    if (msg->detections.empty()) return;

    double Loc_X = 0;
    double Loc_Y = 0;
    double Loc_Yaw_X = 0;
    double Loc_Yaw_Y = 0;
    double Loc_Weight_Sum = 0;

    //Lookup Base -> Cam Transform
    tf::StampedTransform tf_base_cam;
    try{
        tf_listener.lookupTransform("base", camera_frame, ros::Time(0), tf_base_cam);
    }
    catch (tf::TransformException ex) {
        ROS_ERROR("%s",ex.what());
    }

    //Loop all detections
    for(int i = 0; i < msg->detections.size(); i++) {
        int ID;
        tf::Vector3 tag_cam_v;
        tf::Quaternion tag_cam_q;

        ID = msg->detections[i].id[0];
        tf::pointMsgToTF(msg->detections[i].pose.pose.pose.position, tag_cam_v);
        tf::quaternionMsgToTF(msg->detections[i].pose.pose.pose.orientation, tag_cam_q);

        //Transform Coordinate
        tf::Vector3   base_cam_v = tf_base_cam.getOrigin();
        tf::Matrix3x3 base_cam_r(tf_base_cam.getRotation());

        tf::Matrix3x3 tag_cam_r(tag_cam_q);

        tf::Matrix3x3 tag_base_r = tag_cam_r*(base_cam_r.transpose());
        tf::Vector3   tag_base_v = tag_cam_v-tag_base_r*base_cam_v;

        //Tag To Base
        tf::Quaternion tag_base_q;
        tf::Transform  tag_base_transform;

        tag_base_r.getRotation(tag_base_q);
        tag_base_transform.setRotation(tag_base_q);
        tag_base_transform.setOrigin(tag_base_v);

        //Base To Map
        tf::Transform tf_map_base;
        tf_map_base = TagTransforms[ID] * tag_base_transform;

        //Send Current Tag Location
        tf_broadcaster.sendTransform(tf::StampedTransform(tf_map_base, ros::Time::now(), "map", "base_vloc_tag_" + to_string(ID)));

        /* ros::Time tf_time = ros::Time::now();

        //Send TF
        tf::Quaternion tag_base_q;
        tf::Transform  tag_base_transform;
        tag_base_r.getRotation(tag_base_q);
        tag_base_transform.setRotation(tag_base_q);
        tag_base_transform.setOrigin(tag_base_v);
        tf_broadcaster.sendTransform(tf::StampedTransform(tag_base_transform, tf_time, "tag_" + to_string(ID), "base_vloc_tag_" + to_string(ID)));

        //Lookup TF
        tf::StampedTransform tf_world_base;
        try {
            tf_listener.waitForTransform("map", "base_vloc_tag_" + to_string(ID), tf_time, ros::Duration(0.1), ros::Duration(0.001));
            tf_listener.lookupTransform ("map", "base_vloc_tag_" + to_string(ID), tf_time, tf_world_base);
        }
        catch (tf::TransformException ex) {
            ROS_ERROR("%s",ex.what());
        } */

        double Cur_Base_X =   tf_map_base.getOrigin().getX();
        double Cur_Base_Y =   tf_map_base.getOrigin().getY();
        double Cur_Base_Z =   tf_map_base.getOrigin().getZ();
        double Cur_Base_Yaw = YawFromQuaternion(tf_map_base.getRotation());

        //Ignore Flying Base (>0.2m)
        if(std::abs(Cur_Base_Z) < 0.2) {
            //Calc Weight
            double Cur_Weight = 1 / tag_cam_v.length();

            //Sum Up Position Estimate
            Loc_X +=   Cur_Weight * Cur_Base_X;
            Loc_Y +=   Cur_Weight * Cur_Base_Y;
            Loc_Yaw_X += Cur_Weight * cos(Cur_Base_Yaw);
            Loc_Yaw_Y += Cur_Weight * sin(Cur_Base_Yaw);
            Loc_Weight_Sum += Cur_Weight;
        }
    }

    if(!Loc_Weight_Sum) return;

    //get weighted pose (map -> base)
    tf::Transform map_base_tf;
    map_base_tf.setOrigin(tf::Vector3(
        Filter_X.filter(Loc_X / Loc_Weight_Sum),
        Filter_Y.filter(Loc_Y / Loc_Weight_Sum),
        0
    ));
    map_base_tf.setRotation(tf::createQuaternionFromYaw(atan2(
        Filter_Yaw_Y.filter(Loc_Yaw_Y / Loc_Weight_Sum),
        Filter_Yaw_X.filter(Loc_Yaw_X / Loc_Weight_Sum)
    )));

    //get base -> odom
    tf::StampedTransform base_odom_tf;
    try {
        tf_listener.lookupTransform("base", "odom", ros::Time(0), base_odom_tf);
    }
    catch (tf::TransformException ex) {
        ROS_ERROR("%s",ex.what());
    }

    //publish map -> odom
    tf_broadcaster.sendTransform(tf::StampedTransform(map_base_tf * base_odom_tf, ros::Time::now(), "map", "odom"));
}

void VLocalization::TagDetection_Front_Callback(const apriltag_ros::AprilTagDetectionArray::ConstPtr& msg) {
    FuseDetectedTags(msg, "cam_front");
}

void VLocalization::TagDetection_Back_Callback(const apriltag_ros::AprilTagDetectionArray::ConstPtr& msg) {
    FuseDetectedTags(msg, "cam_back");
}
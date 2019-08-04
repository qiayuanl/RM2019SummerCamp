#include "vlocalization.h"

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
    detect_sub = node_priv.subscribe<apriltag_ros::AprilTagDetectionArray> ("tag_detections", 100, &VLocalization::TagDetectionCallback, this);
    vloc_pub   = node_priv.advertise<geometry_msgs::Pose>                  ("vloc", 100);
}

void VLocalization::TagDetectionCallback(const apriltag_ros::AprilTagDetectionArray::ConstPtr& msg) {
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
        tf_listener.lookupTransform("base", "cam", ros::Time(0), tf_base_cam);
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

        //Time
        ros::Time tf_time = ros::Time::now();

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
            tf_listener.lookupTransform("map", "base_vloc_tag_" + to_string(ID), tf_time, tf_world_base);
        }
        catch (tf::TransformException ex) {
            ROS_ERROR("%s",ex.what());
        }

        double Cur_Base_X =   tf_world_base.getOrigin().getX();
        double Cur_Base_Y =   tf_world_base.getOrigin().getY();
        double Cur_Base_Z =   tf_world_base.getOrigin().getZ();
        double Cur_Base_Yaw = YawFromQuaternion(tf_world_base.getRotation());

        //Ignore Flying Base (>0.25m)
        if(std::abs(Cur_Base_Z) < 0.25) {
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

    //Broadcast Pose
    geometry_msgs::Pose pose;

    pose.position.x = Filter_X.filter(Loc_X / Loc_Weight_Sum);
    pose.position.y = Filter_Y.filter(Loc_Y / Loc_Weight_Sum);
    pose.position.z = 0;

    geometry_msgs::Quaternion orientation_quat = tf::createQuaternionMsgFromYaw(atan2(
        Filter_Yaw_Y.filter(Loc_Yaw_Y / Loc_Weight_Sum),
        Filter_Yaw_X.filter(Loc_Yaw_X / Loc_Weight_Sum)
    ));
    pose.orientation = orientation_quat;

    vloc_pub.publish(pose);

    //Broadcast Pose TF
    tf::Transform pose_tf;
    tf::Vector3 pose_position_tf;
    tf::Quaternion pose_orientation_tf;

    tf::pointMsgToTF(pose.position, pose_position_tf);
    tf::quaternionMsgToTF(pose.orientation, pose_orientation_tf);

    pose_tf.setOrigin(pose_position_tf);
    pose_tf.setRotation(pose_orientation_tf);

    tf_broadcaster.sendTransform(tf::StampedTransform(pose_tf, ros::Time(0), "map", "base_vodom"));
}
/* #include "ros/ros.h"
#include "apriltag_ros/AprilTagDetectionArray.h"
#include <tf/transform_listener.h>
#include <tf/transform_broadcaster.h>
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
    tf::StampedTransform tag_cam_transform;

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

        tag_cam_transform.setOrigin( tf::Vector3(tag_cam_x, tag_cam_y, 0.0) );
        tag_cam_transform.setRotation(tag_cam_q);

        br.sendTransform(tf::StampedTransform(tag_cam_transform, ros::Time::now(), "base", "cam"));

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
 */
#include "ros/ros.h"
#include "std_msgs/String.h"

/**
 * This tutorial demonstrates simple receipt of messages over the ROS system.
 */
void chatterCallback(const std_msgs::String::ConstPtr& msg)
{
  ROS_INFO("I heard: [%s]", msg->data.c_str());
}

int main(int argc, char **argv)
{
  /**
   * The ros::init() function needs to see argc and argv so that it can perform
   * any ROS arguments and name remapping that were provided at the command line.
   * For programmatic remappings you can use a different version of init() which takes
   * remappings directly, but for most command-line programs, passing argc and argv is
   * the easiest way to do it.  The third argument to init() is the name of the node.
   *
   * You must call one of the versions of ros::init() before using any other
   * part of the ROS system.
   */
  ros::init(argc, argv, "listener");

  /**
   * NodeHandle is the main access point to communications with the ROS system.
   * The first NodeHandle constructed will fully initialize this node, and the last
   * NodeHandle destructed will close down the node.
   */
  ros::NodeHandle n;

  /**
   * The subscribe() call is how you tell ROS that you want to receive messages
   * on a given topic.  This invokes a call to the ROS
   * master node, which keeps a registry of who is publishing and who
   * is subscribing.  Messages are passed to a callback function, here
   * called chatterCallback.  subscribe() returns a Subscriber object that you
   * must hold on to until you want to unsubscribe.  When all copies of the Subscriber
   * object go out of scope, this callback will automatically be unsubscribed from
   * this topic.
   *
   * The second parameter to the subscribe() function is the size of the message
   * queue.  If messages are arriving faster than they are being processed, this
   * is the number of messages that will be buffered up before beginning to throw
   * away the oldest ones.
   */
  ros::Subscriber sub = n.subscribe("chatter", 1000, chatterCallback);

  /**
   * ros::spin() will enter a loop, pumping callbacks.  With this version, all
   * callbacks will be called from within this thread (the main one).  ros::spin()
   * will exit when Ctrl-C is pressed, or the node is shutdown by the master.
   */
  ros::spin();

  return 0;
}



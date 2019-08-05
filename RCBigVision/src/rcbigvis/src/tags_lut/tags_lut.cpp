#include "ros/ros.h"

#include <tf/tf.h>
#include <tf2_ros/static_transform_broadcaster.h>

inline std::string to_string(int x) {
    std::stringstream ss;
    ss << x;
    return ss.str();
}

int main(int argc, char **argv)
{
	ros::init(argc, argv, "tags_lut");

	ros::NodeHandle nh;

    tf2_ros::StaticTransformBroadcaster broadcaster;

    //Init Static Transforms
    for(int x = 0; x < 7; x++) {
        for(int y = 0; y < 9; y++) {
            int tag_id   = x * 9 + y;
            double tag_x = 0.93 * x;
            double tag_y = 0.93 * y;

            geometry_msgs::TransformStamped tf_map_tag;
            tf_map_tag.header.frame_id = "map";
            tf_map_tag.header.stamp = ros::Time::now();
            tf_map_tag.child_frame_id = "tag_" + to_string(tag_id);

            tf_map_tag.transform.translation.x = tag_x;
            tf_map_tag.transform.translation.y = tag_y;
            tf_map_tag.transform.translation.z = 0.0;

            tf::quaternionTFToMsg(
                tf::createQuaternionFromRPY(0, 0, M_PI / 2),
                tf_map_tag.transform.rotation
            ); //[roll, pitch, yaw]

            broadcaster.sendTransform(tf_map_tag);
        }
    }

    ros::spin();
	return 0;
}
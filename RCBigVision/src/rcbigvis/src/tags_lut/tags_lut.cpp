#include "ros/ros.h"

#include <tf/tf.h>
#include <tf/transform_broadcaster.h>

inline std::string to_string(int x) {
    std::stringstream ss;
    ss << x;
    return ss.str();
}

class TagsBroadcaster {
private:
    tf::TransformBroadcaster tf_broadcaster;

public:
    void update(ros::Time frame_time) {
        for(int x = 0; x < 7; x++) {
            for(int y = 0; y < 9; y++) {
                int tag_id   = y * 7 + x;
                double tag_x = 0.93 * x;
                double tag_y = 0.93 * y;

                tf::Transform tf_map_tag;
                tf_map_tag.setOrigin(tf::Vector3(tag_x, tag_y, 0)); //[x, y, z]
                tf_map_tag.setRotation(tf::createQuaternionFromRPY(0, 0, M_PI / 2)); //[roll, pitch, yaw]

                tf_broadcaster.sendTransform(tf::StampedTransform(tf_map_tag, frame_time, "map", "tag_" + to_string(tag_id)));
            }
        }
    }
};

int main(int argc, char **argv)
{
	ros::init(argc, argv, "tags_lut");

	ros::NodeHandle nh;

    ros::Rate loop_rate(10);

    TagsBroadcaster broadcaster;

	//Process Jobs
	while(ros::ok()) {
        //update subnodes
        broadcaster.update(ros::Time::now() + loop_rate.expectedCycleTime());

        //sleep
        ros::spinOnce();
        loop_rate.sleep();
    }
	return 0;
}
#include <ros/ros.h>
#include <tf/tf.h>

#include <std_msgs/Float64MultiArray.h>

ros::Subscriber servo_sub;

#define VAL_LIMIT(x, minv, maxv) std::min(maxv, std::max(x, minv))

void CallbackServo(const std_msgs::Float64MultiArray::ConstPtr &servo_angle) {
    std::stringstream ss;

    ss << "{";
    for(int i = 0; i < servo_angle->data.size(); i++) {
        double angle = VAL_LIMIT(servo_angle->data[i], 0.0, 1.0);

        int servo_angle = (int)(500.0 + 2000.0 * angle);
        ss << "#" << std::setfill('0') << std::setw(3) << i << "P" << std::setw(4) << servo_angle << "T0000!";
    }
    ss << "}";

    puts(ss.str().c_str());
}

int main(int argc, char **argv)
{
	ros::init(argc, argv, "robot");

	ros::NodeHandle nh;

    servo_sub = nh.subscribe<std_msgs::Float64MultiArray>("servo", 100, CallbackServo);

    ros::spin();
	return 0;
}
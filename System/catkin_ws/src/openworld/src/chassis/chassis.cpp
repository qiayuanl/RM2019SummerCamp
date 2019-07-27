#include <ros/ros.h>
#include <vector>
#include <string>
#include <cmath>
#include <tf/tf.h>

#include <geometry_msgs/Twist.h>
#include <nav_msgs/Odometry.h>
#include <std_msgs/Float64MultiArray.h>

#include <dynamic_reconfigure/server.h>
#include "openworld/ChassisConfig.h"

#include "motor.h"
#include "config.h"

/* Constants */
const double MOTOR_WATCHDOG_TIMEOUT = 1.0; //seconds

class Chassis {
	public:
		Chassis();

		void update();

		void update_odometry();
		void publish_odometry();

		void callback_velocity( const geometry_msgs::Twist::ConstPtr& twist );
		void update_callback_velocity_watchdog();

		void callback_debug(one_bot::ChassisConfig &config, uint32_t level);
		void update_debug();
	private:
		/*
			Config
		*/
		bool Config_IsDebug;

		/*
			Handles
		*/
		ros::NodeHandle nh;
		ros::Publisher	odom_pub;
		ros::Subscriber twist_sub;

		/* 
		 * Motor
		 */
		MotorController motors;
		ros::Time       motorWatchdog;

		/*
		 * Odometry
		 */
		double x, y, theta;
		double lastx, lasty, lasttheta;
		ros::Time lastt;

		double last_position[4];

		/*
		 * Debug
		 */
		ros::Publisher debug_pub;
		dynamic_reconfigure::Server<one_bot::ChassisConfig>               debug_server;
		dynamic_reconfigure::Server<one_bot::ChassisConfig>::CallbackType debug_f;
};

Chassis::Chassis() {
	ros::NodeHandle node_priv("~");

	twist_sub = nh.subscribe<geometry_msgs::Twist>("velocity", 10, &Chassis::callback_velocity, this);
	odom_pub	= nh.advertise<nav_msgs::Odometry>("odom", 50);

	//Motor Controller Config
	double _Kp, _Ki, _Kd, _KmaxI;

	node_priv.param<double>("Kp", _Kp, 1.0);
	node_priv.param<double>("Ki", _Ki, 0.0);
	node_priv.param<double>("Kd", _Kd, 0.0);
	node_priv.param<double>("KmaxI", _KmaxI, 1000.0);

	//Create Motor Controller
	motors.setCoefficients(_Kp, _Ki, _Kd, _KmaxI);

	//Motor Watchdog
	motorWatchdog = ros::Time::now();

	//Odomtery Config
	x = y = theta = 0;
	lastx = lasty = lasttheta = 0;
	lastt = ros::Time::now();

	for(int i = 0; i < 4; i++) last_position[i] = motors.readPosition(i);

	//Debug Config
	node_priv.param<bool>("IsDebug", Config_IsDebug, true);
	if(Config_IsDebug) {
		debug_pub = nh.advertise<std_msgs::Float64MultiArray>("debug", 50);

		debug_f = boost::bind(&Chassis::callback_debug, this, _1, _2);
		debug_server.setCallback(debug_f);
	}
}

void Chassis::update() {
	motors.update();

	update_callback_velocity_watchdog();

	update_odometry();

	if(Config_IsDebug) update_debug();
}

void Chassis::update_odometry() {
	double d[4];

	//calculate delta
	for(int id = 0; id < 4; id++) {
		d[id] = motors.readPosition(id) - last_position[id];
		last_position[id] = motors.readPosition(id);
	}

	double k = CHASSIS_WHEEL_R / 4.0;
	double dx = k * (d[0] + (-d[1]) + d[2] + (-d[3]));
	double dy = k * (d[0] - (-d[1]) - d[2] + (-d[3]));
	double dtheta = k / (CHASSIS_LENGTH_A + CHASSIS_LENGTH_B) * (-d[0] + (-d[1]) - d[2] + (-d[3]));

	x += dx * cos(theta) - dy * sin(theta);
	y += dx * sin(theta) + dy * cos(theta);

	theta += dtheta;
	theta = fmod(theta, 2 * M_PI);

	publish_odometry();
}

void Chassis::publish_odometry() {
	// since all odometry is 6DOF we'll need a quaternion created from yaw
	geometry_msgs::Quaternion odom_quat = tf::createQuaternionMsgFromYaw(theta);

	nav_msgs::Odometry odom;
	odom.header.stamp = ros::Time::now();
	odom.header.frame_id = "odom";

	// set the position
	odom.pose.pose.position.x = x;
	odom.pose.pose.position.y = y;
	odom.pose.pose.position.z = 0.0;
	odom.pose.pose.orientation = odom_quat;

	//calculate velocity
	double dt = (ros::Time::now() - lastt).toSec();
	if(dt == 0) dt = 1e-8; //avoid zero dt

	double dx = x - lastx;
	double dy = y - lasty;
	double dtheta = theta - lasttheta;

	lastx = x;
	lasty = y;
	lasttheta = theta;

	// set the velocity
	odom.child_frame_id = "base_link";
	odom.twist.twist.linear.x = dx / dt;
	odom.twist.twist.linear.y = dy / dt;
	odom.twist.twist.angular.z = dtheta / dt;

	// publish the message
	odom_pub.publish(odom);
}

void Chassis::callback_velocity( const geometry_msgs::Twist::ConstPtr& twist ) {
	//reset watchdog
	motorWatchdog = ros::Time::now();

	//set motor power
	double vx = twist->linear.x;
	double vy = twist->linear.y;
	double vw = twist->angular.z;

	double a = CHASSIS_LENGTH_A + CHASSIS_LENGTH_B;
	motors.setVelocity(0,  (-a * vw + vx - vy) / CHASSIS_WHEEL_R   );
	motors.setVelocity(1, -(( a * vw + vx + vy) / CHASSIS_WHEEL_R) );
	motors.setVelocity(2,  (-a * vw + vx + vy) / CHASSIS_WHEEL_R   );
	motors.setVelocity(3, -(( a * vw + vx - vy) / CHASSIS_WHEEL_R) );
}

void Chassis::update_callback_velocity_watchdog() {
	//Check timeout
	if( (ros::Time::now() - motorWatchdog).toSec() > MOTOR_WATCHDOG_TIMEOUT ) {

		//Zero motor powers
		for(int i = 0; i < 4; i++) {
			motors.setVelocity(i, 0);
		}
	}
}

void Chassis::callback_debug(one_bot::ChassisConfig &config, uint32_t level) {
	motors.setCoefficients(config.Kp, config.Ki, config.Kd, config.KmaxI);

	ROS_INFO("Chassis Reconfigure: [Kp = %lf, Ki = %lf, Kd = %lf, KmaxI = %lf]", 
		config.Kp, config.Ki, config.Kd, config.KmaxI
	);
}

void Chassis::update_debug() {
	std_msgs::Float64MultiArray motorError;

	for(int i = 0; i < 4; i++) {
		motorError.data.push_back( motors.readVelocity(i) - motors.readVelocitySetpoint(i) );
	}

	debug_pub.publish(motorError);
}

int main(int argc, char **argv)
{
	ros::init(argc, argv, "chassis");

	Chassis chassis;

	ros::Rate loop_rate(200);

	while (ros::ok())
	{
		chassis.update();

		ros::spinOnce();
		loop_rate.sleep();
	}

	return 0;
}
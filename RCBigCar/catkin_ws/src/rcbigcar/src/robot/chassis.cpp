#include "chassis.h"

double YawFromQuaternion(const geometry_msgs::Quaternion &quat) {
    tf::Quaternion q_orient(quat.x, quat.y, quat.z, quat.w);
	tf::Matrix3x3  m_orient(q_orient);

    double roll, pitch, yaw;
    m_orient.getRPY(roll, pitch, yaw);

    return yaw;
}

Chassis::Chassis()
{
	ros::NodeHandle node_priv;

	//Setup Variables
	//Setup Motors
	for (int i = 0; i < 4; i++)
	{
		motors[i] = new Motor(i, &MOTOR_CHASSIS, CLOSELOOP_VELOCITY);
	}

	//Setup Paramters
	node_priv.param<bool>("IsDebug", Config_IsDebug, true);

	//Setup Reconfigurable Paramters
	static ros::NodeHandle DynamicParamNodeHandle("~/chassis");
	static dynamic_reconfigure::Server<rcbigcar::ChassisConfig> DynamicParamServer(DynamicParamNodeHandle);
	DynamicParamServer.setCallback(boost::bind(&Chassis::CallbackDynamicParam, this, _1, _2));

	//Setup Comm
	twist_sub   = node_priv.subscribe<geometry_msgs::Twist>("velocity", 100, &Chassis::CallbackVelocity, this);
	vloc_sub    = node_priv.subscribe<geometry_msgs::Pose> ("vloc", 100,     &Chassis::CallbackVLocalization, this);
	pos_pub	    = node_priv.advertise<nav_msgs::Odometry>  ("odom", 100);

	//Setup Odom
	InitialPoseGot = false;
	GyroCorrection = 0.0;

	x = y = theta = 0;
	lastx = lasty = lasttheta = 0;
	lastt = ros::Time::now();
	for (int i = 0; i < 4; i++)
	{
		last_position[i] = motors[i]->getPosition();
	}

	//Setup Watchdog
	motorWatchdog = ros::Time::now();

	//Setup Debug
	if (Config_IsDebug)
	{
		dbg_spd_setpoint_pub = node_priv.advertise<std_msgs::Float64MultiArray>("dbg_set_spd", 50);
		dbg_spd_real_pub = node_priv.advertise<std_msgs::Float64MultiArray>("dbg_real_spd", 50);
	}
}

Chassis::~Chassis()
{
	//Free Motors
	for (int i = 0; i < 4; i++)
	{
		delete motors[i];
	}
}

void Chassis::update()
{
	//Update Motors
	for (int i = 0; i < 4; i++)
	{
		motors[i]->update();
	}

	//Update Modules
	UpdateWatchdog();
	UpdateOdometry();
	UpdateDebug();
}

double Chassis::ReadGyroAngle() {
	return -(Hardware()->gyro.angle / 180 * M_PI);
}

void Chassis::CallbackVLocalization( const geometry_msgs::Pose::ConstPtr& pose ) {
	//Transform Quat -> RPY
	/* tf::Quaternion q_orient(pose->orientation.x, pose->orientation.y, pose->orientation.z, pose->orientation.w);
	tf::Matrix3x3  m_orient(q_orient);

    double roll, pitch, yaw;
    m_orient.getRPY(roll, pitch, yaw); */

	//Update Coordinate
	x = pose->position.x;
	y = pose->position.y;

	//Correct Gyro
	GyroCorrection = -ReadGyroAngle() + YawFromQuaternion(pose->orientation);

	//Set Initial Pose
	InitialPoseGot = true;
}

void Chassis::UpdateOdometry() {
	//Must get initial position
	if(!InitialPoseGot) {
		return;
	}

	double d[4];

	//calculate delta
	for (int id = 0; id < 4; id++)
	{
		d[id] = motors[id]->getPosition() - last_position[id];
		last_position[id] = motors[id]->getPosition();
	}

	double k = CHASSIS_WHEEL_R / 4.0;
	double dx = k * (-d[0] + d[1] + d[2] - d[3]);
	double dy = k * (-d[0] - d[1] + d[2] + d[3]);
	double dtheta = k / (CHASSIS_LENGTH_A + CHASSIS_LENGTH_B) * (-d[0] - d[1] - d[2] - d[3]);

	x += dx * cos(theta) - dy * sin(theta);
	y += dx * sin(theta) + dy * cos(theta);

	//theta += dtheta;
	theta = ReadGyroAngle() + GyroCorrection;
	theta = fmod(theta, 2 * M_PI);

	PublishPosition();
}

void Chassis::PublishPosition() {
	// since all odometry is 6DOF we'll need a quaternion created from yaw
	geometry_msgs::Quaternion odom_quat = tf::createQuaternionMsgFromYaw(theta);

	nav_msgs::Odometry odom;
	odom.header.stamp = ros::Time::now();
	odom.header.frame_id = "world";

	// set the position
	odom.pose.pose.position.x = x;
	odom.pose.pose.position.y = y;
	odom.pose.pose.position.z = 0.0;
	odom.pose.pose.orientation = odom_quat;

	//calculate velocity
	double dt = (ros::Time::now() - lastt).toSec();
	if (dt == 0)
		dt = 1e-8; //avoid zero dt

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
	pos_pub.publish(odom);
}

void Chassis::CallbackVelocity(const geometry_msgs::Twist::ConstPtr &twist)
{
	//reset watchdog
	motorWatchdog = ros::Time::now();

	//set motor power
	double vx = twist->linear.x;
	double vy = twist->linear.y;
	double vw = twist->angular.z;

	double a = CHASSIS_LENGTH_A + CHASSIS_LENGTH_B;

	double w[4] = {
		-((a * vw + vx + vy) / CHASSIS_WHEEL_R),
		((-a * vw + vx - vy) / CHASSIS_WHEEL_R),
		(-a * vw + vx + vy) / CHASSIS_WHEEL_R,
		-((a * vw + vx - vy) / CHASSIS_WHEEL_R)};

	//Velocity Limitation
	double maxVel = 0.0;
	for (int i = 0; i < 4; i++)
		maxVel = std::max(maxVel, std::abs(w[i]));

	if (maxVel > Dyn_Config_MaxVel)
	{
		double factor = Dyn_Config_MaxVel / maxVel;
		for (int i = 0; i < 4; i++)
			w[i] *= factor;
	}

	//Send Velocity
	for (int i = 0; i < 4; i++)
		motors[i]->Setpoint = w[i];
}

void Chassis::UpdateWatchdog()
{
	//Check timeout
	if ((ros::Time::now() - motorWatchdog).toSec() > CHASSIS_WATCHDOG_TIMEOUT)
	{

		//Zero motor powers
		for (int i = 0; i < 4; i++)
		{
			motors[i]->Setpoint = 0;
		}
	}
}

void Chassis::CallbackDynamicParam(rcbigcar::ChassisConfig &config, uint32_t level)
{
	//Dynamic Params
	Dyn_Config_MaxVel = config.MaxVel;

	//Dynamic Motor Params
	for (int i = 0; i < 4; i++)
	{
		motors[i]->setCoefficients(config.Kp, config.Ki, config.Kd, config.Kf, config.KmaxI);
	}

	ROS_INFO("Chassis Reconfigure: [Kp = %lf, Ki = %lf, Kd = %lf, Kf = %lf, KmaxI = %lf, MaxVel = %lf]",
			 config.Kp, config.Ki, config.Kd, config.Kf, config.KmaxI, config.MaxVel);
}

void Chassis::UpdateDebug()
{
	if (!Config_IsDebug)
		return;

	std_msgs::Float64MultiArray motorSetpoint;
	std_msgs::Float64MultiArray motorReal;

	for (int i = 0; i < 4; i++)
	{
		motorSetpoint.data.push_back(motors[i]->Setpoint);
		motorReal.data.push_back(motors[i]->getVelocity());
	}

	dbg_spd_setpoint_pub.publish(motorSetpoint);
	dbg_spd_real_pub.publish(motorReal);
}
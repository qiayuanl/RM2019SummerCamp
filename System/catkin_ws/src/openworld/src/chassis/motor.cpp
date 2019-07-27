#include "motor.h"

MotorController::MotorController() {
	//Initialize Time
	last_looptime = ros::Time(0);

	//Initialize Motors
	power.resize(MOTOR_COUNT);
	encoder.resize(MOTOR_COUNT);
	last_encoder.resize(MOTOR_COUNT);

	real_vel.resize(MOTOR_COUNT);
	set_vel.resize(MOTOR_COUNT);

	//Initialize PID
	Kp = 0;
	Ki = 0;
	Kd = 0;
	KmaxI = 1000;

	VError.resize(MOTOR_COUNT);
	VError_Filtered.resize(MOTOR_COUNT);
	VError_Derivative.resize(MOTOR_COUNT);
	VError_Derivative_Filtered.resize(MOTOR_COUNT);
	VError_Intergral.resize(MOTOR_COUNT);
}

MotorController::~MotorController() {
	I2CClose();
}

void MotorController::setCoefficients(double _Kp, double _Ki, double _Kd, double _KmaxI) {
	Kp = _Kp;
	Ki = _Ki;
	Kd = _Kd;
	KmaxI = _KmaxI;
}

void MotorController::setVelocity(int id, double vel) {
	set_vel[id] = vel;
}

double MotorController::readVelocity(int id) {
	return real_vel[id];
}

double MotorController::readVelocitySetpoint(int id) {
	return set_vel[id];
}

double MotorController::readPosition(int id) {
	return encoder[id] * MOTOR_TICKS_TO_RADIANS;
}

void MotorController::update() {
	if(last_looptime.isZero()) {
		//Return at zero start time
		last_looptime = ros::Time::now();
		return;
	}

	//get Time
	double dt = (ros::Time::now() - last_looptime).toSec();
	last_looptime = ros::Time::now();

	if(!dt) {
		//Return at zero delta time
		return;
	}

	//calculate velocity
	for(int id = 0; id < MOTOR_COUNT; id++) {
		real_vel[id] = (encoder[id] - last_encoder[id]) * MOTOR_TICKS_TO_RADIANS / dt;
		last_encoder[id] = encoder[id];
	}

	//PID Closeloop
	//Reference: https://bitbucket.org/AndyZe/pid
	double c_ = 1.0; //filter error paramter 1/4 sampling rate

	for(int id = 0; id < MOTOR_COUNT; id++) {
		//Calculate Error
		double error = set_vel[id] - real_vel[id];
	
		VError[id].value[2] = VError[id].value[1];
		VError[id].value[1] = VError[id].value[0];
		VError[id].value[0] = error;

		//intergrate error
		VError_Intergral[id] += VError[id].value[0] * dt;
		if(VError_Intergral[id] > KmaxI)
			VError_Intergral[id] = KmaxI;
		if(VError_Intergral[id] < -KmaxI)
			VError_Intergral[id] = -KmaxI;

		VError_Filtered[id].value[2] = VError_Filtered[id].value[1];
		VError_Filtered[id].value[1] = VError_Filtered[id].value[0];
		VError_Filtered[id].value[0] = (1 / (1 + c_ * c_ + 1.414 * c_)) * (VError[id].value[2] + 2 * VError[id].value[1] + VError[id].value[0] -
																	(c_ * c_ - 1.414 * c_ + 1) * VError_Filtered[id].value[2] -
																	(-2 * c_ * c_ + 2) * VError_Filtered[id].value[1]);

		//calculate error derivative
		VError_Derivative[id].value[2] = VError_Derivative[id].value[1];
		VError_Derivative[id].value[1] = VError_Derivative[id].value[0];
		VError_Derivative[id].value[0] = (VError[id].value[0] - VError[id].value[1]) / dt;

		//filter error derivative
		VError_Derivative_Filtered[id].value[2] = VError_Derivative_Filtered[id].value[1];
		VError_Derivative_Filtered[id].value[1] = VError_Derivative_Filtered[id].value[0];

		VError_Derivative_Filtered[id].value[0] =
			(1 / (1 + c_ * c_ + 1.414 * c_)) *
			(VError_Derivative[id].value[2] + 2 * VError_Derivative[id].value[1] + VError_Derivative[id].value[0] -
			(c_ * c_ - 1.414 * c_ + 1) * VError_Derivative_Filtered[id].value[2] - (-2 * c_ * c_ + 2) * VError_Derivative_Filtered[id].value[1]);

		//output power
		double output = Kp * VError_Filtered[id].value[0]
					+ Ki * VError_Intergral[id] 
					+ Kd * VError_Derivative_Filtered[id].value[0];

		power[id] = output * MOTOR_PWM_RESOLUTION;

		if(power[id] >  MOTOR_PWM_RESOLUTION)  power[id] =  MOTOR_PWM_RESOLUTION;
		if(power[id] < -MOTOR_PWM_RESOLUTION)  power[id] = -MOTOR_PWM_RESOLUTION;
	}
}
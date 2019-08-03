#include "motor.h"

Motor::Motor(int _ID, const MotorPreset *_Preset, MotorCloseloopType _CloseloopType)
{
	//Initialize Self
	ID = _ID;
	Preset = _Preset;
	CloseloopType = _CloseloopType;

	//Initialize Time
	last_looptime = ros::Time(0);

	//Initialize Setpoint
	Setpoint = 0;

	//Initialize PID
	Kp = 0;
	Ki = 0;
	Kd = 0;
	Kf = 1.0;
	KmaxI = 1000;

	//Initialize PID Variables
	VError_Intergral = 0;
}

void Motor::setCoefficients(double _Kp, double _Ki, double _Kd, double _Kf, double _KmaxI)
{
	Kp = _Kp;
	Ki = _Ki;
	Kd = _Kd;
	Kf = _Kf;
	KmaxI = _KmaxI;
}

double Motor::getVelocity()
{
	return Hardware()->motors[ID].speed_rpm * Preset->RPMToRad;
}

double Motor::getPosition()
{
	return Hardware()->motors[ID].total_angle * Preset->TickToRad;
}

void Motor::update()
{
	if (last_looptime.isZero())
	{
		//Return at zero start time
		last_looptime = ros::Time::now();
		return;
	}

	//get Time
	double dt = (ros::Time::now() - last_looptime).toSec();
	last_looptime = ros::Time::now();

	if (!dt)
	{
		//Return at zero delta time
		return;
	}

	//PID Closeloop
	//Reference: https://bitbucket.org/AndyZe/pid

	// Check if tan(_) is really small, could cause c = NaN
	double tan_filt_ = tan((Kf * 6.2832) * dt / 2);

	// Avoid tan(0) ==> NaN
	if ((tan_filt_ <= 0.) && (tan_filt_ > -0.01)) tan_filt_ = -0.01;
	if ((tan_filt_ >= 0.) && (tan_filt_ < 0.01))  tan_filt_ = 0.01;

	double c_ = 1 / tan_filt_;

	//Calculate Error
	double real = (CloseloopType == CLOSELOOP_VELOCITY) ? getVelocity() : getPosition();
	double error = Setpoint - real;

	VError.value[2] = VError.value[1];
	VError.value[1] = VError.value[0];
	VError.value[0] = error;

	//intergrate error
	VError_Intergral += VError.value[0] * dt;
	if (VError_Intergral > KmaxI)
		VError_Intergral = KmaxI;
	if (VError_Intergral < -KmaxI)
		VError_Intergral = -KmaxI;

	VError_Filtered.value[2] = VError_Filtered.value[1];
	VError_Filtered.value[1] = VError_Filtered.value[0];
	VError_Filtered.value[0] = (1 / (1 + c_ * c_ + 1.414 * c_)) * (VError.value[2] + 2 * VError.value[1] + VError.value[0] -
																   (c_ * c_ - 1.414 * c_ + 1) * VError_Filtered.value[2] -
																   (-2 * c_ * c_ + 2) * VError_Filtered.value[1]);

	//calculate error derivative
	VError_Derivative.value[2] = VError_Derivative.value[1];
	VError_Derivative.value[1] = VError_Derivative.value[0];
	VError_Derivative.value[0] = (VError.value[0] - VError.value[1]) / dt;

	//filter error derivative
	VError_Derivative_Filtered.value[2] = VError_Derivative_Filtered.value[1];
	VError_Derivative_Filtered.value[1] = VError_Derivative_Filtered.value[0];

	VError_Derivative_Filtered.value[0] =
		(1 / (1 + c_ * c_ + 1.414 * c_)) *
		(VError_Derivative.value[2] + 2 * VError_Derivative.value[1] + VError_Derivative.value[0] -
		 (c_ * c_ - 1.414 * c_ + 1) * VError_Derivative_Filtered.value[2] - (-2 * c_ * c_ + 2) * VError_Derivative_Filtered.value[1]);

	//output power
	double output = Kp * VError_Filtered.value[0] + Ki * VError_Intergral + Kd * VError_Derivative_Filtered.value[0];

	int pwm_max_value = Preset->PWMMaxValue;
	int out_power = (int)(output * pwm_max_value);

	if (out_power > pwm_max_value)
		out_power = pwm_max_value;
	if (out_power < -pwm_max_value)
		out_power = -pwm_max_value;

	Hardware()->motors[ID].power = out_power;
}
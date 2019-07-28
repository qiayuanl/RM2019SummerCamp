#include "motor.h"

/////////////////////////////Hardware Level//////////////////////////////
void MotorController::HWUpdateOffset(moto_measure_t *ptr, can_frame_t* frame) {
    ptr->last_angle = ptr->angle;
    ptr->angle = (uint16_t)(frame->data[0] << 8 | frame->data[1]);
    ptr->real_current  = (int16_t)(frame->data[2]<<8 | frame->data[3]);
    ptr->speed_rpm = ptr->real_current;
    ptr->given_current = (int16_t)(frame->data[4]<<8 | frame->data[5]) / - 5;
    ptr->hall = frame->data[6];

    if(ptr->angle - ptr->last_angle > 4096)
        ptr->round_cnt --;
    else if (ptr->angle - ptr->last_angle < -4096)
        ptr->round_cnt ++;

    ptr->total_angle = ptr->round_cnt * 8192 + ptr->angle - ptr->offset_angle;
}

void MotorController::HWUpdateTotalAngle(moto_measure_t *p) {
    int res1, res2, delta;
    if(p->angle < p->last_angle) {
        res1 = p->angle + 8192 - p->last_angle;
        res2 = p->angle - p->last_angle;
    }
    else {
        res1 = p->angle - 8192 - p->last_angle;
        res2 = p->angle - p->last_angle;
    }

    if(abs(res1) < abs(res2))
        delta = res1;
    else
        delta = res2;

    p->total_angle += delta;
    p->last_angle = p->angle;
}

void MotorController::HWReceiveFrame(can_frame_t *frame) {
    switch(frame->can_id)
    {
        case CAN_3510Moto1_ID:
        case CAN_3510Moto2_ID:
        case CAN_3510Moto3_ID:
        case CAN_3510Moto4_ID:
        case CAN_3510Moto5_ID:
        case CAN_3510Moto6_ID:
        case CAN_3510Moto7_ID:
        case CAN_3510Moto8_ID:
        {
                uint8_t idx = frame->can_id - CAN_3510Moto1_ID;
                HWUpdateOffset(&motors[idx], frame);
				HWUpdateTotalAngle(&motors[idx]);

				motors[idx].msg_cnt++;
                break;
        }

		/*
        case SINGLE_GYRO_ID:
        {
            single_gyro.angle = 0.001f * ((int32_t)( (frame->data[0] << 24) | (frame->data[1] << 16) | (frame->data[2] << 8) | (frame->data[3]) ) );
            single_gyro.gyro = 0.001f *  ((int32_t)( (frame->data[4] << 24) | (frame->data[5] << 16) | (frame->data[6] << 8) | (frame->data[7]) ) );

            break;
        }
		*/
    }
}

void ReceiveHandlerProxy(can_frame_t *frame, void *ptr) {
	((MotorController *)ptr)->HWReceiveFrame(frame);
}
/////////////////////////////////////////////////////////////////////////

MotorController::MotorController() {
	//Initialize Time
	last_looptime = ros::Time(0);

	//Initialize Motors
	motors.resize(MOTOR_COUNT);

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

	//Initialize Hardware
	adapter = new SocketCAN();
	adapter->reception_handler = &ReceiveHandlerProxy;
	adapter->reception_handler_data = (void *)this;

    adapter->open(CHASSIS_CAN_ID);
}

MotorController::~MotorController() {
	adapter->close();
	delete adapter;
}

void MotorController::setCoefficients(double _Kp, double _Ki, double _Kd, double _KmaxI) {
	Kp = _Kp;
	Ki = _Ki;
	Kd = _Kd;
	KmaxI = _KmaxI;
}

void MotorController::setVelocity(int id, double vel) {
	motors[id].set_vel = vel;
}

double MotorController::readVelocitySetpoint(int id) {
	return motors[id].set_vel;
}

double MotorController::readVelocity(int id) {
	return motors[id].speed_rpm * CHASSIS_RPM_TO_METERS;
}

double MotorController::readPosition(int id) {
	return motors[id].total_angle * CHASSIS_TICK_TO_METERS;
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

	//PID Closeloop
	//Reference: https://bitbucket.org/AndyZe/pid
	double c_ = 1.0; //filter error paramter 1/4 sampling rate

	for(int id = 0; id < MOTOR_COUNT; id++) {
		//Calculate Error
		double error = readVelocitySetpoint(id) - readVelocity(id);
	
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

		int power = (int)(output * MOTOR_PWM_RESOLUTION);

		if(power >  MOTOR_PWM_RESOLUTION)  power =  MOTOR_PWM_RESOLUTION;
		if(power < -MOTOR_PWM_RESOLUTION)  power = -MOTOR_PWM_RESOLUTION;

		motors[id].power = power;
	}

	//Send Motor Power
	if(adapter->is_open()) {
		can_frame_t frame;

		//Transmit Frame 1
		frame.can_id = CHASSIS_CAN_MOTOR_ID_1;
		frame.can_dlc = 8;

		for(int id = 0; id < std::min(MOTOR_COUNT, 4); id++) {
			int16_t power = (int16_t)motors[id].power;

			frame.data[2 * id] =     (uint8_t)(power >> 8);
			frame.data[2 * id + 1] = (uint8_t)(power);
		}

		adapter->transmit(&frame);

		//Transmit Frame 2
		if(MOTOR_COUNT > 4) {
			frame.can_id = CHASSIS_CAN_MOTOR_ID_2;
			frame.can_dlc = 8;

			for(int id = 4; id < MOTOR_COUNT; id++) {
				int16_t power = (int16_t)motors[id].power;

				frame.data[2 * id] =     (uint8_t)(power >> 8);
				frame.data[2 * id + 1] = (uint8_t)(power);
			}

			adapter->transmit(&frame);
		}
	}
}
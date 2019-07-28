#ifndef __ONEBOT_CONFIG__
#define __ONEBOT_CONFIG__

#include <cmath>

/*
 * Mechanical Measurements (SI Unit)
 */
const double CHASSIS_WHEEL_R = 0.038;
const double CHASSIS_LENGTH_A = 0.25;
const double CHASSIS_LENGTH_B = 0.25;

const double CHASSIS_MOTOR_PROPOTION = 36;
const double CHASSIS_RPM_TO_METERS = (1.0 / 60.0) * 2.0 * M_PI * CHASSIS_WHEEL_R / CHASSIS_MOTOR_PROPOTION;
const double CHASSIS_TICK_TO_METERS = (1.0 / 8192.0) * 2.0 * M_PI * CHASSIS_WHEEL_R / CHASSIS_MOTOR_PROPOTION;

/*
 * Motor Controller
 */
const int MOTOR_COUNT = 4;
const int MOTOR_PWM_RESOLUTION = 10000;

#define CHASSIS_CAN_MOTOR_ID_1 0x200
#define CHASSIS_CAN_MOTOR_ID_2 0x1FF
#define CHASSIS_CAN_ID "can0"

#endif
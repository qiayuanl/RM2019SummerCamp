#ifndef __ONEBOT_CONFIG__
#define __ONEBOT_CONFIG__

#include <cmath>

/* 
 * Preset Types
 */
enum MotorCloseloopType
{
    CLOSELOOP_VELOCITY,
    CLOSELOOP_POSITION
};

struct MotorPreset
{
    int PWMMaxValue;
    double RPMToRad;
    double TickToRad;
};

struct MotionMotor
{
    int ID;
    MotorCloseloopType CloseloopType;
    const MotorPreset *Preset;
};

/* 
 * Robot Hardware Config
 */

#define ROBOT_SAMPLING_RATE 200

#define HW_MOTOR_COUNT 8
#define HW_CAN_MOTOR_ID_1 0x200
#define HW_CAN_MOTOR_ID_2 0x1FF
#define HW_CAN_ID "can0"

const double MOTOR_CALIBRATION_THRESHOLD = 0.628;
const double MOTOR_CALIBRATION_DURATION  = 0.5;
const double MOTOR_CALIBRATION_POWER     = 0.1;

/*
 * Different Motor Presets
 */
const MotorPreset MOTOR_GM2006 = {
    10000,                                           //PWM Max Value
    (1.0 / 60.0) * 2.0 * M_PI / 36.0,                //RPM To rad/s
    (1.0 / 8192.0) * 2.0 * M_PI / 36.0               //TICK to Meters
};

const MotorPreset MOTOR_GM3508 = {
    16384,
    (1.0 / 60.0) * 2.0 * M_PI / (3591.0 / 187.0),
    (1.0 / 8192.0) * 2.0 * M_PI / (3591.0 / 187.0)};

const MotorPreset MOTOR_GM3510 = {
    29000,
    (1.0 / 60.0) * 2.0 * M_PI,
    (1.0 / 8192.0) * 2.0 * M_PI};

/*
 * Chassis Paramters (SI Unit)
 */

#define MOTOR_CHASSIS MOTOR_GM2006

const double CHASSIS_WATCHDOG_TIMEOUT = 1.0;

const double CHASSIS_WHEEL_R = 0.038;
const double CHASSIS_LENGTH_A = 0.25;
const double CHASSIS_LENGTH_B = 0.25;

/*
 * Motion Paramters
 */
#define MOTION_MOTOR_COUNT 2
const double MOTION_WATCHDOG_TIMEOUT = 1.0;

const MotionMotor MOTION_MOTOR_PRESET[MOTION_MOTOR_COUNT] = {
    {
        4,
        CLOSELOOP_POSITION,
        &MOTOR_GM3510,
    },
    {
        5,
        CLOSELOOP_VELOCITY,
        &MOTOR_GM3508,
    }
};

#endif

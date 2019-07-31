#ifndef __ONEBOT_CONFIG__
#define __ONEBOT_CONFIG__

#include <cmath>

/* 
 * Preset Types
 */
struct MotorPreset {
    int PWMResolution;
    double RPMToRad;
    double TickToRad;

    MotorPreset(
        int _PWMResolution,
        double _RPMToRad,
        double _TickToRad
    ) {
        PWMResolution = _PWMResolution;
        RPMToRad      = _RPMToRad;
        TickToRad     = _TickToRad;
    }
};

/* 
 * Robot Hardware Config
 */

#define HW_MOTOR_COUNT         8
#define HW_CAN_MOTOR_ID_1      0x200
#define HW_CAN_MOTOR_ID_2      0x1FF
#define HW_CAN_ID              "can0"

/*
 * Different Motor Presets
 */
const MotorPreset MOTOR_CHASSIS(
    10000,                               //PWM Resolution
    (1.0 / 60.0) * 2.0 * M_PI / 36.0,    //RPM To rad/s
    (1.0 / 8192.0) * 2.0 * M_PI / 36.0   //TICK to Meters
);

/*
 * Chassis Paramters (SI Unit)
 */

const double CHASSIS_WATCHDOG_TIMEOUT = 1.0;

const double CHASSIS_WHEEL_R = 0.038;
const double CHASSIS_LENGTH_A = 0.25;
const double CHASSIS_LENGTH_B = 0.25;

#endif

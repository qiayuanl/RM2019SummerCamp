#ifndef __MOTOR_H__
#define __MOTOR_H__

#include "hardware.h"

struct ErrorList {
    double value[3];
};

class Motor {
public:
    Motor(int _ID, const MotorPreset *_Preset, int _SetType);

    void update();

    void setCoefficients(double _Kp, double _Ki, double _Kd, double _Kf, double _KmaxI);

    double getVelocity();
    double getPosition();

    /* Setpoint */
    int SetType;

    double Setpoint;

private:
    /* ID  */
    int ID;

    /* Preset */
    const MotorPreset *Preset;

    /* Time */
    ros::Time last_looptime;

    /* PID */
    double Kp, Ki, Kd, Kf, KmaxI;

    /* PID Variables */
    ErrorList VError = {{0}};
    ErrorList VError_Filtered = {{0}};
    ErrorList VError_Derivative = {{0}};
    ErrorList VError_Derivative_Filtered = {{0}};
    double    VError_Intergral = 0;
};

#endif
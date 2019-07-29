#ifndef __MOTOR_H__
#define __MOTOR_H__

#include "hardware.h"

struct ErrorList {
    double value[3];
};

class Motor {
public:
    Motor(int _ID, const MotorPreset *_Preset);

    void update();

    void setCoefficients(double _Kp, double _Ki, double _Kd, double _Kf, double _KmaxI);

    double getVelocity();
    double getPosition();

    /* Setpoint */
    double SetVelocity;

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
    ErrorList VError;
    ErrorList VError_Filtered;
    ErrorList VError_Derivative;
    ErrorList VError_Derivative_Filtered;
    double    VError_Intergral;
};

#endif
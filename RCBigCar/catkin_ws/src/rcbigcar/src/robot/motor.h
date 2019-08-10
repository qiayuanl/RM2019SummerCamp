#ifndef __MOTOR_H__
#define __MOTOR_H__

#include "hardware.h"

struct ErrorList
{
    double value[3];
};

class Motor
{
  public:
    Motor(int _ID, const MotorPreset *_Preset, MotorParamter _Paramter);

    void update();

    void setCoefficients(double _Kp, double _Ki, double _Kd, double _Kf, double _KmaxI, double _KmaxO);

    double getVelocity();
    double getPosition();

    /* Setpoint */
    double Setpoint;

    /* Paramter */
    MotorParamter Paramter;
  private:
    /* ID  */
    int ID;

    /* Preset */
    const MotorPreset *Preset;

    /* Time */
    ros::Time last_looptime;

    /* PID */
    double Kp, Ki, Kd, Kf, KmaxI, KmaxO;

    /* PID Variables */
    ErrorList VError = {{0}};
    ErrorList VError_Filtered = {{0}};
    ErrorList VError_Derivative = {{0}};
    ErrorList VError_Derivative_Filtered = {{0}};
    double VError_Intergral = 0;

    /* Calibration */
    bool isCalibrating;
    double CalibrateDuration;
    double CalibrationValue;
};

#endif
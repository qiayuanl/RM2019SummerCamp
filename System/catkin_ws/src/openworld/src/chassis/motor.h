#ifndef __MOTOR_H__
#define __MOTOR_H__

#include "config.h"

#include <ros/ros.h>
#include <vector>

const double MOTOR_TICKS_TO_RADIANS = 2.0 * M_PI / MOTOR_TICKS_PER_REVOLUTION;

struct ErrorList {
    double value[3];
};

class MotorController {
  public:
    MotorController();
    ~MotorController();

    void update();

    void setVelocity(int id, double vel);
    double readVelocity(int id);
    double readVelocitySetpoint(int id);
    double readPosition(int id);

    void setCoefficients(double _Kp, double _Ki, double _Kd, double _KmaxI);

    void EncoderInterrupt(int id);

  private:
    /* Time */
    ros::Time last_looptime;

    /* Motors */
    std::vector<int> power;
    std::vector<int> encoder;
    std::vector<int> last_encoder;

    std::vector<double> real_vel;
    std::vector<double> set_vel;

    /* PID */
    double Kp, Ki, Kd, KmaxI;

    std::vector<ErrorList> VError;
    std::vector<ErrorList> VError_Filtered;
    std::vector<ErrorList> VError_Derivative;
    std::vector<ErrorList> VError_Derivative_Filtered;
    std::vector<double> VError_Intergral;

    /* Hardware */
    int hI2C;
    void I2COpen();
    void I2CClose();
    void I2CWrite(uint8_t address, uint8_t value);

    void PCA9685SetFreq(uint16_t freq);
    void PCA9685SetValue(uint8_t id, uint16_t on, uint16_t off);
};

#endif
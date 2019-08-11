#ifndef MECHANICAL_EXECUTER_H
#define MECHANICAL_EXECUTER_H

#include <ros/ros.h>
#include <std_msgs/Float64MultiArray.h>

namespace MechanicalExecuter {
    const int QUEUE_NUMBER = 5;
    const double ANGULAR_TOLERANCE = 5.0 / 180.0 * M_PI; //rad

    enum ActionType {
        ACTION_NONE,

        ACTION_SERVO,
        ACTION_MOTOR
    };

    struct Action {
        ActionType type;

        int actutator_id;
        double setpoint;

        ros::Time target_time;
    };

    //Comm
    ros::Subscriber motor_status_sub;
    ros::Publisher  motor_setpoint_pub;
    ros::Publisher  servo_setpoint_pub;

    std_msgs::Float64MultiArray MotorStatus;
    std_msgs::Float64MultiArray MotorSetpoint;

    std_msgs::Float64MultiArray ServoSetpoint;

    //Action List
    std::vector<Action> MechanicalActions[QUEUE_NUMBER];

    bool IsBusy() {
        for(int i = 0; i < QUEUE_NUMBER; i++) {
            if(!MechanicalActions[i].empty())
                return true;
        }

        return false;
    }

    void update() {
        for(int i = 0; i < QUEUE_NUMBER; i++) if(!MechanicalActions[i].empty()) {
            bool Finished = false;

            //get first action
            Action &CurAction = *MechanicalActions[i].begin();

            //send setpoint
            switch(CurAction.type) {
                case ACTION_SERVO:
                    //send setpoint
                    if(CurAction.actutator_id > (int)ServoSetpoint.data.size() - 1) {
                        ServoSetpoint.data.resize(CurAction.actutator_id + 1);
                    }

                    ServoSetpoint.data[CurAction.actutator_id] = CurAction.setpoint;

                    servo_setpoint_pub.publish(ServoSetpoint);

                    //wait countdown
                    Finished = ros::Time::now() >= CurAction.target_time;
                break;

                case ACTION_MOTOR:
                    //send setpoint
                    if(CurAction.actutator_id > (int)MotorSetpoint.data.size() - 1) {
                        MotorSetpoint.data.resize(CurAction.actutator_id + 1);
                    }

                    MotorSetpoint.data[CurAction.actutator_id] = CurAction.setpoint;

                    motor_setpoint_pub.publish(MotorSetpoint);

                    //wait for angle
                    Finished = false;

                    if((3 * CurAction.actutator_id) <= (int)MotorStatus.data.size() - 1) {
                        Finished = fabs(CurAction.setpoint - MotorStatus.data[3 * CurAction.actutator_id]) < ANGULAR_TOLERANCE;
                    }

                    //timeout
                    Finished |= ros::Time::now() >= CurAction.target_time;
                break;

                default:
                    Finished = true;
                break;
            }

            //remove if finished
            if(Finished) MechanicalActions[i].erase(MechanicalActions[i].begin());
        }
    }

    void CallbackMotorStatus(const std_msgs::Float64MultiArray::ConstPtr &motor_status) {
        MotorStatus = *motor_status;
    }

    void reset() {
        for(int i = 0; i < QUEUE_NUMBER; i++) {
            MechanicalActions[i].clear();
        }
    }

    void init() {
        ros::NodeHandle nh;

        servo_setpoint_pub = nh.advertise<std_msgs::Float64MultiArray>("servo", 100);
        motor_setpoint_pub = nh.advertise<std_msgs::Float64MultiArray>("motor", 100);

        motor_status_sub   = nh.subscribe<std_msgs::Float64MultiArray>("motorstatus", 100, &CallbackMotorStatus);

        reset();
    }
}

#endif

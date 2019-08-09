#ifndef MECHANICAL_EXECUTER_H
#define MECHANICAL_EXECUTER_H

#include <ros/ros.h>
#include <std_msgs/Float64MultiArray.h>

namespace MechanicalExecuter {
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
    std::vector<Action> MechanicalActions;

    bool IsBusy() {
        return !MechanicalActions.empty();
    }

    void update() {
        if(MechanicalActions.empty()) return;

        bool Finished = false;

        //get first action
        Action &CurAction = *MechanicalActions.begin();

        //send setpoint
        switch(CurAction.type) {
            case ACTION_SERVO:
                //send setpoint
                if(CurAction.actutator_id > ServoSetpoint.data.size() - 1) {
                    ServoSetpoint.data.resize(CurAction.actutator_id + 1);
                }

                ServoSetpoint.data[CurAction.actutator_id] = CurAction.setpoint;

                servo_setpoint_pub.publish(ServoSetpoint);

                //wait countdown
                Finished = ros::Time::now() >= CurAction.target_time;
            break;

            case ACTION_MOTOR:
                //send setpoint
                if(CurAction.actutator_id > MotorSetpoint.data.size() - 1) {
                    MotorSetpoint.data.resize(CurAction.actutator_id + 1);
                }

                MotorSetpoint.data[CurAction.actutator_id] = CurAction.setpoint;

                motor_setpoint_pub.publish(MotorSetpoint);

                //wait for angle
                Finished = false;

                int data_index = 3 * CurAction.actutator_id;
                if(data_index <= MotorStatus.data.size() - 1) {
                    Finished = fabs(CurAction.setpoint - MotorStatus.data[data_index]) < ANGULAR_TOLERANCE;
                }
            break;

            default:
                Finished = true;
            break;
        }

        //remove if finished
        if(Finished) MechanicalActions.erase(MechanicalActions.begin());
    }

    void CallbackMotorStatus(const std_msgs::Float64MultiArray::ConstPtr &motor_status) {
        MotorStatus = *motor_status;
    }

    void init() {
        ros::NodeHandle nh;

        servo_setpoint_pub = nh.advertise<std_msgs::Float64MultiArray>("servo", 100);
        motor_setpoint_pub = nh.advertise<std_msgs::Float64MultiArray>("motor", 100);

        motor_status_sub   = nh.subscribe<std_msgs::Float64MultiArray>("motorstatus", 100, &CallbackMotorStatus);

        reset();
    }

    void reset() {
        MechanicalActions.clear();
    }
}

#endif
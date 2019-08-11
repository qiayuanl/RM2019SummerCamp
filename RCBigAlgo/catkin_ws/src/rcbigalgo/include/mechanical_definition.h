#ifndef MECHANICAL_DEFINITION_H
#define MECHANICAL_DEFINITION_H

#include "mechanical_executer.h"

namespace MechanicalDefinition {
    const int    MAIN_SCREW_POLE_ID = 0;
    const double MAIN_SCREW_POLE_UP_ANGLE = -85.0;
    const double MAIN_SCREW_POLE_DOWN_ANGLE = -5.0;

    const int    BALL_MOTOR_IDS[2]       = {1, 2};
    const double BALL_MOTOR_IN_ANGLE     = -1.7;
    const double BALL_MOTOR_OUT_ANGLE    = -2.7;

    const int    CUP_BALL_SERVO_ID = 0;
    const double CUP_BALL_SERVO_ANGLE[3] = {0.48, 0.65, 0.82};

    int CupBallCount[3];
    int BallCount;
    int CupCount;

    void init() {
        CupCount = 10;

        CupBallCount[0] = 4;
        CupBallCount[1] = 3;
        CupBallCount[2] = 4;

        BallCount = 10;
    }

    bool PlaceBall() {
        if(MechanicalExecuter::IsBusy()) return false;

        if(!BallCount) return false;
        BallCount--;

        //ball go in
        MechanicalExecuter::MechanicalActions[0].push_back({
            MechanicalExecuter::ACTION_MOTOR, //type

            BALL_MOTOR_IDS[0], //id
            BALL_MOTOR_IN_ANGLE, //angle
            ros::Time::now() + ros::Duration(1.0) //end time
        });
        MechanicalExecuter::MechanicalActions[1].push_back({
            MechanicalExecuter::ACTION_MOTOR, //type

            BALL_MOTOR_IDS[1], //id
            -BALL_MOTOR_IN_ANGLE, //angle
            ros::Time::now() + ros::Duration(1.0) //end time
        });

        //ball go out
        MechanicalExecuter::MechanicalActions[0].push_back({
            MechanicalExecuter::ACTION_MOTOR, //type

            BALL_MOTOR_IDS[0], //id
            BALL_MOTOR_OUT_ANGLE, //angle
            ros::Time::now() + ros::Duration(1.0) //end time
        });
        MechanicalExecuter::MechanicalActions[1].push_back({
            MechanicalExecuter::ACTION_MOTOR, //type

            BALL_MOTOR_IDS[1], //id
            -BALL_MOTOR_OUT_ANGLE, //angle
            ros::Time::now() + ros::Duration(1.0) //end time
        });

        return true;
    }

    //@TODO: W.I.P
    bool PlaceCup() {
      return PlaceBall();
    }

    /*bool PlaceCup() {
        return PlaceBall();

         if(MechanicalExecuter::IsBusy()) return false;

        if(!CupCount) return false;
        CupCount--;

        //release a cup

        //screw pole move in

        //scratch a cup

        //screw pole move out & cup rotate
        for(int i = 0; i < 3; i++) if(CupBallCount[i]) {
            CupBallCount[i]--;

            MechanicalExecuter::MechanicalActions[0].push_back({
                MechanicalExecuter::ACTION_SERVO, //type

                CUP_BALL_SERVO_ID, //id
                CUP_BALL_SERVO_ANGLE[i], //angle
                ros::Time::now() + ros::Duration(1.0) //end time
            });

            MechanicalExecuter::MechanicalActions[0].push_back({
                MechanicalExecuter::ACTION_SERVO, //type

                CUP_BALL_SERVO_ID, //id
                0.0, //angle
                ros::Time::now() + ros::Duration(1.3) //end time
            });
        }

        return true;
    }*/
}

#endif

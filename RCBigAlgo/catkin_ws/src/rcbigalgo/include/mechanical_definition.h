#ifndef MECHANICAL_DEFINITION_H
#define MECHANICAL_DEFINITION_H

#include "mechanical_executer.h"

namespace MechanicalDefinition {
    const double CUP_BALL_SERVO_ANGLE[3] = {0.48, 0.65, 0.82};

    int CupBallCount[3];
    int BallCount[3];

    void init() {
        reset();
    }

    void reset() {
        CupBallCount[0] = 4;
        CupBallCount[1] = 3;
        CupBallCount[2] = 4;

        BallCount[0] = BallCount[1] = 10;
    }

    bool PlaceBall() {
        if(MechanicalExecuter::IsBusy()) return false;

        for(int i = 0; i < 3; i++) if(CupBallCount[i]) {
            CupBallCount[i]--;

            MechanicalExecuter::MechanicalActions.push_back({
                ACTION_SERVO, //type

                0, //id
                CUP_BALL_SERVO_ANGLE[i], //angle
                ros::Time::now() + ros::Duration(1.0) //end time
            });

            MechanicalExecuter::MechanicalActions.push_back({
                ACTION_SERVO, //type

                0, //id
                0.0, //angle
                ros::Time::now() + ros::Duration(1.3) //end time
            });
        }

        return false;
    }
}

#endif
#ifndef ACTION_EXECUTER
#define ACTION_EXECUTER

#include "global_planner.h"

namespace ActionExecuter {
    ros::Subscriber pos_sub;
    ros::Publisher  chassis_setpoint_pub;

    GlobalPlanner::ActionList ActionList;
    GlobalPlanner::Action     CurAction;
    int                       CurCountdown = 0;

    void update() {
        //check finished
        bool finished = false;

        if(!finished) return;

        //check out next action
        GlobalPlanner::Action LastAction = CurAction;

    }
}

#endif
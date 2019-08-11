#ifndef ACTION_EXECUTER
#define ACTION_EXECUTER

#include "global_planner.h"
#include "mechanical_definition.h"
#include "mechanical_executer.h"

#include <tf/tf.h>
#include <geometry_msgs/Pose.h>
#include <geometry_msgs/PoseStamped.h>
#include <nav_msgs/Odometry.h>

//position threshold
const double POSITION_THRESHOLD = 0.05;
//angle threshold
const double ANGLE_THRESHOLD = 5.0 / 180.0 * M_PI;
//speed threshold
const double POSITION_SPEED_THRESHOLD = 0.03;
const double ANGLE_SPEED_THRESHOLD    = 10.0 / 180.0 * M_PI;

double YawFromQuaternion(const tf::Quaternion &q_orient)
{
    tf::Matrix3x3 m_orient(q_orient);

    double roll, pitch, yaw;
    m_orient.getRPY(roll, pitch, yaw);

    return yaw;
}

double AngularMinus(double a, double b)
{
    a = fmod(a, 2.0 * M_PI);
    b = fmod(b, 2.0 * M_PI);

    double res1 = a - b;
    double res2 = (a < b) ? (a + 2 * M_PI - b) : (a - 2 * M_PI - b);

    return (std::abs(res1) < std::abs(res2)) ? res1 : res2;
}

namespace ActionExecuter
{
    //Communication
    ros::Publisher  setpoint_pub;

    //Pose & Velocity
    struct Pose {
        double x, y, yaw;
        ros::Time stamp;
    } Pose, PoseLast, Velocity;
    bool PoseInitialized = false;

    //Pose Setpoint
    geometry_msgs::PoseStamped setpoint_pose;

    //Actions
    GlobalPlanner::ActionList ActionList;
    GlobalPlanner::Action CurAction;
    ros::Time CurActionLastTime;

    void LoadActionList(const GlobalPlanner::ActionList &_ActionList) {
        ActionList = _ActionList;

        CurAction.type = GlobalPlanner::ACTION_NONE;
    }

    void UpdatePose() {
        //Read Pose
        static tf::TransformListener tf_listener;
        tf::StampedTransform tf_map_base;

        try {
            tf_listener.lookupTransform("map", "base", ros::Time(0), tf_map_base);
        }
        catch (tf::TransformException ex) {
            ROS_ERROR("%s",ex.what());
            return;
        }

        //Current Pose
        PoseInitialized = true;

        Pose.x   = tf_map_base.getOrigin().getX();
        Pose.y   = tf_map_base.getOrigin().getY();
        Pose.yaw = YawFromQuaternion(tf_map_base.getRotation());
        Pose.stamp = ros::Time::now();

        //Velocity
        double dt = (Pose.stamp - PoseLast.stamp).toSec();
        Velocity.x = (Pose.x - PoseLast.x) / dt;
        Velocity.y = (Pose.y - PoseLast.y) / dt;
        Velocity.yaw = AngularMinus(Pose.yaw, PoseLast.yaw) / dt;

        PoseLast = Pose;
    }

    bool CurActionFinished()
    {
        double dt = (ros::Time::now() - CurActionLastTime).toSec();
        CurActionLastTime = ros::Time::now();

        if (CurAction.type == GlobalPlanner::ACTION_NONE)
            return true;

        //Initial Pose
        if(!PoseInitialized) return false;

        //Mechanical Busy
        if(MechanicalExecuter::IsBusy())
            return false;

        //ZeroVel Timeout
        if( 
            (fabs(Velocity.x)   < POSITION_SPEED_THRESHOLD) &&
            (fabs(Velocity.y)   < POSITION_SPEED_THRESHOLD) &&
            (fabs(Velocity.yaw) < ANGLE_SPEED_THRESHOLD)
        ) {
            //stable
            CurAction.zero_countdown -= dt;
        }

        //if not zero speed
        if(CurAction.zero_countdown > 0) {
          //Angle settings
          if (CurAction.yaw_enabled) {
              if (fabs( AngularMinus(CurAction.world_yaw, Pose.yaw ) ) > ANGLE_THRESHOLD)
                  return false;
          }

          //Position settings
          if(
              (fabs( Pose.x - CurAction.world_x ) > POSITION_THRESHOLD) ||
              (fabs( Pose.y - CurAction.world_y ) > POSITION_THRESHOLD)
          ) {
              return false;
          }
        }

        //Normal Timeout
        if(CurAction.countdown > 0) CurAction.countdown -= dt;
        if(CurAction.countdown <= 0) return true;

        //Wait for feedback
        bool Feedback_Done = true;

        switch(CurAction.type) {
            case GlobalPlanner::ACTION_OCCUPY:
                Feedback_Done = GlobalBoard.board_state(CurAction.feedback.grid_x, CurAction.feedback.grid_y) == WhoAmI;
            break;

            case GlobalPlanner::ACTION_PLACEBALL:
            case GlobalPlanner::ACTION_PLACECUP:
                Feedback_Done = GlobalBoard.castle_we_placed[CurAction.feedback.castle_id] != CurAction.feedback.castle_last_value;
            break;
        }

        if(!Feedback_Done) return false;

        //Else complete
        return true;
    }

    void update()
    {
        UpdatePose();

        //broadcast pose
        if(CurAction.type != GlobalPlanner::ACTION_NONE) {
            setpoint_pose.pose.position.x = CurAction.world_x;
            setpoint_pose.pose.position.y = CurAction.world_y;
            setpoint_pose.pose.position.z = 0;

            if(CurAction.yaw_enabled) {
                setpoint_pose.pose.orientation = tf::createQuaternionMsgFromYaw(CurAction.world_yaw);
            }

            setpoint_pose.header.stamp = ros::Time::now();
            setpoint_pose.header.frame_id = "map";
            setpoint_pub.publish( setpoint_pose );
        }

        //check finished
        if (!CurActionFinished())
            return;

        //get next action
        if(ActionList.empty()) return;

        CurAction = *ActionList.begin();
        ActionList.erase(ActionList.begin());

        //init next action
        CurActionLastTime = ros::Time::now();

        //do mechanical ops
        if(CurAction.type == GlobalPlanner::ACTION_PLACEBALL) {
            MechanicalDefinition::PlaceBall();
        }
        else if(CurAction.type == GlobalPlanner::ACTION_PLACECUP) {
            MechanicalDefinition::PlaceCup();
        }
    }

    void init() {
        //init mechanical executer & definition
        MechanicalExecuter::init();
        MechanicalDefinition::init();

        //init action executer
        ros::NodeHandle nh;

        setpoint_pub = nh.advertise<geometry_msgs::PoseStamped>("move_base_simple/goal", 100);

        setpoint_pose.pose.orientation = tf::createQuaternionMsgFromYaw(0.0);
    }

    void reset() {
        ActionList.clear();
        CurAction.type = GlobalPlanner::ACTION_NONE;
    }
} // namespace ActionExecuter

#endif

#ifndef GLOBAL_PLANNER_H
#define GLOBAL_PLANNER_H

#include "game.h"
#include <ros/ros.h>
#include <tf/tf.h>
#include <tf/transform_listener.h>

inline std::string to_string(int x) {
    std::stringstream ss;
    ss << x;
    return ss.str();
}

namespace GlobalPlanner {
    enum ActionType {
        ACTION_NONE,

        ACTION_MOVETO,
        ACTION_OCCUPY,

        ACTION_PLACEBALL,
        ACTION_PLACECUP
    };

    struct Action {
        ActionType type;

        double world_x, world_y;
        
        bool yaw_enabled;
        double world_yaw;
    }; 

    typedef std::vector<Action>  ActionList;
    typedef std::vector<uint8_t> StrategyList;

    double XYToWorld[MAX_X][MAX_Y][2];

    inline void Init_XYToWorld() {
        tf::TransformListener tf_listener;
        tf::StampedTransform tf_map_tag;

        for(int x = 0; x < MAX_X; x++) for(int y = 0; y < MAX_Y; y++) {
            try {
                tf_listener.lookupTransform("tag_" + to_string(x * 9 + y), "map", ros::Time::now(), tf_map_tag);
            }
            catch (tf::TransformException ex) {
                ROS_ERROR("%s",ex.what());
            }

            XYToWorld[x][y][0] = tf_map_tag.getOrigin().x;
            XYToWorld[x][y][1] = tf_map_tag.getOrigin().y;
        }
    }

    ActionList GetActions(bool who, const Game::Board &b, const StrategyList& st) {
        ActionList actions;
        
        //Starting point
        int x = b.position[who][0];
        int y = b.position[who][1];

        //Push Starting point
        actions.push_back({
            ACTION_MOVETO,

            XYToWorld[x][y][0], XYToWorld[x][y][1],

            false, 0.0
        });

        for(int i = 0; i < st.size(); i++) {
            uint8_t strategy = st[i];

            //move
            if(strategy >= 0 && strategy <= 3) {
                x += Game::dx[strategy];
                y += Game::dy[strategy];

                actions.push_back({
                    ACTION_MOVETO,

                    XYToWorld[x][y][0], XYToWorld[x][y][1],

                    false, 0.0
                });
            }
            //occupy
            else if(strategy == 4) {
                actions.push_back({
                    ACTION_OCCUPY,

                    XYToWorld[x][y][0], XYToWorld[x][y][1],

                    false, 0.0
                });
            }
            //place ball / cup
            else if((strategy == 5) || (strategy == 6)) {
                //find castle
                int castle_x = -1;
                int castle_y = -1;

                for(int i = 0; i < 4; i++) {
                    int tx = x + Game::dx[i];
                    int ty = y + Game::dy[i];

                    if(Game::CASTLE_ID[tx][ty] != -1) {
                        castle_x = tx;
                        castle_y = ty;

                        break;
                    }
                }

                //castle exists
                if(castle_x != -1) {
                    //calculate yaw
                    double yaw = atan2(
                        XYToWorld[castle_x][castle_y][1] - XYToWorld[x][y][1],
                        XYToWorld[castle_x][castle_y][0] - XYToWorld[x][y][0]
                    );

                    //push
                    actions.push_back({
                        (strategy == 5) ? ACTION_PLACEBALL : ACTION_PLACECUP,

                        XYToWorld[x][y][0], XYToWorld[x][y][1],

                        false, 0.0
                    });
                }
            }
        }

        return actions;
    }
}

#endif
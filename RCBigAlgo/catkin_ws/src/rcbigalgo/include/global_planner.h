#ifndef GLOBAL_PLANNER_H
#define GLOBAL_PLANNER_H

#include "game.h"
#include "global_board.h"
#include <ros/ros.h>

#include <tf/tf.h>
#include <tf/transform_listener.h>

inline std::string to_string(int x) {
    std::stringstream ss;
    ss << x;
    return ss.str();
}

namespace GlobalPlanner {
    const double PLACE_DELTA_DISTANCE = 0.40;
    const double MOVE_ZERO_COUNTDOWN =  1.0;

    enum ActionType {
        ACTION_NONE,

        ACTION_MOVETO,
        ACTION_OCCUPY,

        ACTION_PLACEBALL,
        ACTION_PLACECUP
    };

    struct FeedbackInfo {
        int grid_x, grid_y;

        int castle_id, castle_last_value;
    };

    struct Action {
        ActionType type;

        double world_x, world_y;
        
        bool yaw_enabled; double world_yaw;

        double countdown; //countdown & timeout
        double zero_countdown;
        FeedbackInfo feedback;
    }; 

    typedef std::vector<Action>  ActionList;
    typedef std::vector<uint8_t> StrategyList;

    double XYToWorld[Game::MAX_X][Game::MAX_Y][2];

    inline void Init_XYToWorld() {
        tf::TransformListener tf_listener;
        tf::StampedTransform tf_map_tag;

        for(int x = 0; x < Game::MAX_X; x++) for(int y = 0; y < Game::MAX_Y; y++) {
            try {
                tf_listener.waitForTransform("map", "tag_" + to_string(x * 9 + y), ros::Time(0), ros::Duration(1), ros::Duration(0.001));
                tf_listener.lookupTransform("map", "tag_" + to_string(x * 9 + y), ros::Time(0), tf_map_tag);
            }
            catch (tf::TransformException ex) {
                ROS_ERROR("%s",ex.what());
            }

            XYToWorld[x][y][0] = tf_map_tag.getOrigin().x();
            XYToWorld[x][y][1] = tf_map_tag.getOrigin().y();
        }
    }

    ActionList GetActions(bool who, const Game::Board &b, const StrategyList& st) {
        //init xy to world
        Init_XYToWorld();

        ActionList actions;

        //Starting point
        int x = b.position[who][0];
        int y = b.position[who][1];

        //Push Starting point
        actions.push_back({
            ACTION_MOVETO,

            XYToWorld[x][y][0], XYToWorld[x][y][1],

            false, 0.0,

            0.0, MOVE_ZERO_COUNTDOWN,

            {}
        });

        for(int i = 0; i < st.size(); i++) {
            uint8_t strategy = st[i];

            //move
            if(strategy >= 0 && strategy <= 3) {
                x += Game::dx[strategy];
                y += Game::dy[strategy];

                //get next op
                uint8_t next_instruction = 0xFF;
                if( (i + 1) < st.size() ) next_instruction = st[i + 1];

                //if not eq next op
                if(strategy != next_instruction) {
                    actions.push_back({
                        ACTION_MOVETO,

                        XYToWorld[x][y][0], XYToWorld[x][y][1],

                        false, 0.0,

                        0.0, MOVE_ZERO_COUNTDOWN,

                        {}
                    });
                }
            }
            //occupy
            else if(strategy == 4) {
                actions.push_back({
                    ACTION_OCCUPY,

                    XYToWorld[x][y][0], XYToWorld[x][y][1],

                    false, 0.0,

                    2.0, MOVE_ZERO_COUNTDOWN,

                    {
                        x, y,
                        0, 0
                    }
                });
            }
            //place ball / cup
            else if((strategy == 5) || (strategy == 6)) {
                //find castle
                int castle_dir = -1;
                int castle_id  = -1;

                for(int i = 0; i < 4; i++) {
                    int tx = x + Game::dx[i];
                    int ty = y + Game::dy[i];

                    if(COORD_OK(tx, ty) && (Game::CASTLE_ID[tx][ty] != -1)) {
                        castle_dir = i;

                        castle_id = Game::CASTLE_ID[tx][ty];

                        break;
                    }
                }

                //castle exists
                if(castle_dir != -1) {
                    //calculate yaw & place pos
                    double yaw = Game::diryaw[castle_dir];

                    double place_pos_x = XYToWorld[x][y][0] + cos(yaw) * PLACE_DELTA_DISTANCE;
                    double place_pos_y = XYToWorld[x][y][1] + sin(yaw) * PLACE_DELTA_DISTANCE;

                    //push turn
                    actions.push_back({
                        ACTION_MOVETO,

                        XYToWorld[x][y][0], XYToWorld[x][y][1],

                        true, yaw,

                        0.0, MOVE_ZERO_COUNTDOWN,

                        {}
                    });

                    //push gofront
                    actions.push_back({
                        ACTION_MOVETO,

                        place_pos_x, place_pos_y,

                        false, 0.0,

                        0.0, MOVE_ZERO_COUNTDOWN,

                        {}
                    });

                    //push place
                    ActionType place_action_type = (strategy == 5) ? ACTION_PLACEBALL : ACTION_PLACECUP;
                    actions.push_back({
                        place_action_type,

                        place_pos_x, place_pos_y,

                        false, 0.0,

                        (
                            (place_action_type == ACTION_PLACEBALL)
                             ? (2.0 * Game::ACTION_PLACE_BALL_MS)
                             : (1.5 * Game::ACTION_PLACE_CUP_MS)
                        ),
                        MOVE_ZERO_COUNTDOWN,

                        {
                            x, y,

                            castle_id, GlobalBoard.castle[castle_id]
                        }
                    });

                    //push goback
                    actions.push_back({
                        ACTION_MOVETO,

                        XYToWorld[x][y][0], XYToWorld[x][y][1],

                        false, 0.0,

                        0.0, MOVE_ZERO_COUNTDOWN,

                        {}
                    });
                }
            }
        }

        return actions;
    }
}

#endif

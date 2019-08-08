#ifndef __GAME_THREAD_H__
#define __GAME_THREAD_H__

#include "game.h"
#include <unistd.h>

namespace GameThread {
    enum ActionType {
        ACTION_MOVETO,
        ACTION_OCCUPY,

        ACTION_PLACEBALL,
        ACTION_PLACECUP
    };

    struct Action {
        ActionType type;

        int target_x;
        int target_y;
        double target_w;
    };

    typedef std::vector<Action> ActionList;

    struct GameInfo {
        bool our_team;
        bool who_move;

        int cur_time_left;
        int cur_move_left;

        Game::Board b;
        ActionList action;
    };

    bool ThreadRunning = true;
    bool ActionsUpdated = false;

    GameInfo GlobalGameInfo;
    pthread_mutex_t GameInfoLock;

    void SearchThread(void *param) {
        GameInfo cur_game_info;
        while(ThreadRunning) {
            //read gameinfo
            pthread_mutex_lock(&GameInfoLock);
            cur_game_info = GlobalGameInfo;
            pthread_mutex_unlock(&GameInfoLock);

            //search
            if(cur_game_info.who_move != cur_game_info.our_team) {

            }

            //write actionlist

            usleep(10 * 1000);
        }

        pthread_exit(NULL);
    }
};

#endif
#ifndef __GAME_THREAD_H__
#define __GAME_THREAD_H__

#include "game.h"

namespace GameThread {
    enum ActionType {
        ACTION_MOVETO,
        ACTION_OCCUPY,

        ACTION_PLACEBALL,
        ACTION_PLACECUP
    };

    struct Action {
        int target_x;
        int 
    };

    struct GameInfo {
        bool cur_team;
        bool who_move;
    };

    typedef std::vector<uint8_t> StrategySeq;
};

#endif
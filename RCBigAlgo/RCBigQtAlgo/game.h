#ifndef __GAME_H__
#define __GAME_H__

//ASSET: ME = 0
//       OPP = 1

#include <cstring>
#include <cstdio>
#include <algorithm>

namespace Game {

    //point consts
    const int POINTS_PENALTY_CROSS = 5;
    const int POINTS_HAVE_TOWER = 50;
    const int POINTS_OCCUPY = 15;
    const int POINTS_STRONG_OCCUPY = 20;

    //Consts
    const int MAX_X = 7;
    const int MAX_Y = 9;

    const int CASTLE_ID[MAX_X][MAX_Y] = {
        {-1, -1,  0, -1, -1, -1,  1, -1, -1},
        {-1, -1, -1, -1, -1, -1, -1, -1, -1},
        {-1, -1, -1, -1, -1, -1, -1, -1, -1},
        { 2, -1, -1, -1,  3, -1, -1, -1,  4},
        {-1, -1, -1, -1, -1, -1, -1, -1, -1},
        {-1, -1, -1, -1, -1, -1, -1, -1, -1},
        {-1, -1,  5, -1, -1, -1,  6, -1, -1},
    };

    //Directions
    const int dx[4] = {1, 0, -1, 0};
    const int dy[4] = {0, 1, 0, -1};

    //utility
    inline int sgn(int x) {
        if(x == 0) return 0;
        return (x < 0) ? -1 : 1;
    }
    inline int point_sign(bool x) { return (x == 0) ? 1 : -1; }

    #define COORD_OK(x, y) (((x) >= 0) && ((y) >= 0) && ((x) < MAX_X) && ((y) < MAX_Y))
    #define COORD_SAME(xa, ya, xb, yb) ( ((xa) == (xb)) && ((ya) == (yb)) )

    //Bitboard Ops
    inline int  position  (int x, int y)                      { return x * MAX_Y + y; }
    inline int  get_bit(uint64_t v,  int x, int y)            { int pos = position(x, y); return (v >> pos) & 1; }
    inline void set_bit(uint64_t &v, int x, int y, bool val)  { int pos = position(x, y); v = (v & (~(1ULL << pos))) | (((uint64_t)val) << pos); }

    struct Board {
        //Real States
        // --board
        uint64_t is_occupy;
        uint64_t who;

        // --castle
        int8_t castle[8];

        // --position
        int8_t position[2][2];

        // --carry
        int8_t ball[2], cup[2];

        // --delta mark
        int16_t delta_points;

        //Calculated States
        uint64_t is_strong;

        //Functions
        inline int8_t board_state(int x, int y) {
            return get_bit(is_occupy, x, y) ? get_bit(who, x, y) : -1;
        }
    };

    namespace CC {
        Board *cc_board;

        int8_t ccid[MAX_X][MAX_Y];
        int8_t cccnt;

        int8_t cc_castle[MAX_X * MAX_Y + 5];
        int8_t cc_cur_color;

        void dfs_cc(int x, int y) {
            //Mark cc_id
            ccid[x][y] = cccnt;
            if(CASTLE_ID[x][y] != -1) cc_castle[cccnt]++;

            //Expand
            for(int i = 0; i < 4; i++) {
                int tx = x + dx[i];
                int ty = y + dy[i];
                if(COORD_OK(tx, ty) && !ccid[tx][ty] && (cc_board->board_state(tx, ty) == cc_cur_color)) dfs_cc(tx, ty);
            }
        }

        void recalc_strong(Board *board) {
            //Set board
            cc_board = board;

            //Zeroize Paramters
            cccnt = 0;
            memset(ccid, 0, sizeof(uint8_t) * MAX_X * MAX_Y);

            //DFS
            for(int x = 0; x < MAX_X; x++) for(int y = 0; y < MAX_Y; y++) if(!ccid[x][y]) {
                int8_t color = cc_board->board_state(x, y);
                if(color != -1) {
                    cccnt++;
                    cc_cur_color = color;
                    cc_castle[cccnt] = 0;

                    dfs_cc(x, y);
                }
            }

            //Update
            for(int x = 0; x < MAX_X; x++) for(int y = 0; y < MAX_Y; y++) {
                if(cc_board->board_state(x, y) != -1) {
                    set_bit(cc_board->is_strong, x, y, cc_castle[ccid[x][y]] >= 3);
                }
            }
        }
    };

    namespace OP {
        inline bool can_move(Board &board, bool who, int dir) {
            int tx = board.position[who][0] + dx[dir];
            int ty = board.position[who][1] + dy[dir];

            //not ok or collide another robot or collide castle
            if(!COORD_OK(tx, ty) || COORD_SAME(tx, ty, board.position[!who][0], board.position[!who][1]) || (CASTLE_ID[tx][ty] != -1)) return false;
            return true;
        }

        inline void move(Board &board, bool who, int dir) {
            int tx = board.position[who][0] + dx[dir];
            int ty = board.position[who][1] + dy[dir];

            board.position[who][0] = tx;
            board.position[who][1] = ty;

            //penalty
            if(board.board_state(tx, ty) == (!who)) board.delta_points -= point_sign(who) * POINTS_PENALTY_CROSS;
        }

        inline bool can_occupy(Board &board, bool who) {
            int x = board.position[who][0];
            int y = board.position[who][1];
            
            for(int i = 0; i < 4; i++) {
                int tx = x + dx[i];
                int ty = y + dy[i];

                if(COORD_OK(tx, ty) && (board.board_state(tx, ty) == who)) {
                    return (CASTLE_ID[x][y] == -1) && (board.board_state(x, y) != who) && (!get_bit(board.is_strong, x, y));
                }
            }

            return false;
        }

        inline void occupy(Board &board, bool who) {
            int x = board.position[who][0];
            int y = board.position[who][1];

            set_bit(board.is_occupy, x, y, true);
            set_bit(board.who,       x, y, who);

            CC::recalc_strong(&board);
        }

        inline bool can_place(Board &board, bool who) {
            int x = board.position[who][0];
            int y = board.position[who][1];

            for(int i = 0; i < 4; i++) {
                int tx = x + dx[i];
                int ty = y + dy[i];

                if(COORD_OK(tx, ty) && (CASTLE_ID[tx][ty] != -1)) return true;
            }

            return false;
        }

        inline void place(Board &board, bool who, int8_t cnt) {
            int x = board.position[who][0];
            int y = board.position[who][1];

            for(int i = 0; i < 4; i++) {
                int tx = x + dx[i];
                int ty = y + dy[i];

                if(COORD_OK(tx, ty) && (CASTLE_ID[tx][ty] != -1)) {
                    int castle_id = CASTLE_ID[tx][ty];

                    int sgn_old = sgn(board.castle[castle_id]);
                    board.castle[castle_id] += point_sign(who) * cnt;
                    int sgn_new = sgn(board.castle[castle_id]);

                    if(sgn_old != sgn_new) {
                        //castle state change
                        set_bit(board.is_occupy, tx, ty, (sgn_new != 0));
                        set_bit(board.who,       tx, ty, (sgn_new == 1) ? 0 : 1);

                        CC::recalc_strong(&board);
                    }

                    break;
                }
            }
        }
    };

    namespace Hash {

    };

    namespace Search {

    };
};

#endif

#ifndef __GAME_H__
#define __GAME_H__

//ASSET: ME = 0
//       OPP = 1

//DEFINITIONS OF SEARCH
//1GB  Transposition Table
//128M Hash Table
#define TT_HASH_MOD  40000003
#define TT_SIZE      20000000

//OPTIMIZATION
#pragma GCC optimize(3)
#pragma GCC optimize("Ofast")
#pragma GCC optimize("inline")

//BEGIN
#include <cstring>
#include <cstdio>
#include <algorithm>
#include <vector>
#include <sys/time.h>

#define VAL_LIMIT(x, minv, maxv) std::max( std::min( (x), (maxv) ), (minv) )

namespace Game {
    //time consts
    const int GAME_TOTAL_STEP = 15;
    const int GAME_TOTAL_MS   = 30000;

    const int ACTION_MOVE_MS  = 1000;
    const int ACTION_OCCUPY_MS = 1000;

    const int ACTION_PLACE_BALL_MS = 2000;
    const int ACTION_PLACE_CUP_MS  = 4000;

    //point consts
    const int POINTS_PENALTY_CROSS = 5;
    const int POINTS_HAVE_TOWER = 50;
    const int POINTS_OCCUPY = 15;
    const int POINTS_STRONG_OCCUPY = 20;

    //Consts
    const int MAX_X = 7;
    const int MAX_Y = 9;
    const int MAX_CASTLE = 7;

    const int CASTLE_ID[MAX_X][MAX_Y] = {
        {-1, -1,  0, -1, -1, -1,  1, -1, -1},
        {-1, -1, -1, -1, -1, -1, -1, -1, -1},
        {-1, -1, -1, -1, -1, -1, -1, -1, -1},
        { 2, -1, -1, -1,  3, -1, -1, -1,  4},
        {-1, -1, -1, -1, -1, -1, -1, -1, -1},
        {-1, -1, -1, -1, -1, -1, -1, -1, -1},
        {-1, -1,  5, -1, -1, -1,  6, -1, -1},
    };
    const int CASTLE_COORD[MAX_CASTLE][2] = {
        {0, 2},
        {0, 6},

        {3, 0},
        {3, 4},
        {3, 8},

        {6, 2},
        {6, 6}
    };
    /* const int CASTLE_OCCU_COORD[2][MAX_CASTLE][2] = {{
        {0, 1},
        {0, 5},

        {2, 0},
        {2, 4},
        {4, 8},

        {6, 3},
        {6, 7}
    },
    {
        {0, 3},
        {0, 7},

        {4, 0},
        {4, 4},
        {2, 8},

        {6, 1},
        {6, 5}
    }
    };*/
    const int CASTLE_PLACE_ID[2][MAX_X][MAX_Y] = {
    {
        {-1,  0, -1, -1, -1,  1, -1, -1, -1},
        {-1, -1, -1, -1, -1, -1, -1, -1, -1},
        { 2, -1, -1, -1,  3, -1, -1, -1, -1},
        {-1, -1, -1, -1, -1, -1, -1, -1, -1},
        {-1, -1, -1, -1, -1, -1, -1, -1,  4},
        {-1, -1, -1, -1, -1, -1, -1, -1, -1},
        {-1, -1, -1,  5, -1, -1, -1,  6, -1},
    },
    {
        {-1, -1, -1,  0, -1, -1, -1,  1, -1},
        {-1, -1, -1, -1, -1, -1, -1, -1, -1},
        {-1, -1, -1, -1, -1, -1, -1, -1,  4},
        {-1, -1, -1, -1, -1, -1, -1, -1, -1},
        { 2, -1, -1, -1,  3, -1, -1, -1, -1},
        {-1, -1, -1, -1, -1, -1, -1, -1, -1},
        {-1,  5, -1, -1, -1,  6, -1, -1, -1},
    }
    };

    //Directions
    const int dx[4] = {1, 0, -1, 0};
    const int dy[4] = {0, 1, 0, -1};

/*
    const int dx_8[8] = {1, 0, -1, 0, 1,  1, -1, -1};
    const int dy_8[8] = {0, 1, 0, -1, 1, -1,  1, -1};
*/

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
            int castle_id = CASTLE_PLACE_ID[who][x][y];

            return (castle_id != -1);
        }

        inline void place(Board &board, bool who, int8_t cnt) {
            int x = board.position[who][0];
            int y = board.position[who][1];
            int castle_id = CASTLE_PLACE_ID[who][x][y];

            int sgn_old = sgn(board.castle[castle_id]);
            board.castle[castle_id] += point_sign(who) * cnt;
            int sgn_new = sgn(board.castle[castle_id]);

            if(sgn_old != sgn_new) {
                //castle state change
                int castle_x = CASTLE_COORD[castle_id][0];
                int castle_y = CASTLE_COORD[castle_id][1];

                set_bit(board.is_occupy, castle_x, castle_y, (sgn_new != 0));
                set_bit(board.who,       castle_x, castle_y, (sgn_new == 1) ? 0 : 1);

                CC::recalc_strong(&board);
            }
        }

        inline int get_score(Board &b, bool who) {
            int score = 0;

            //grid
            for(int x = 0; x < MAX_X; x++) for(int y = 0; y < MAX_Y; y++) {
                int8_t occupy_status = b.board_state(x, y);

                if(occupy_status == who) {
                    if(CASTLE_ID[x][y] != -1) score += 50;
                    else score += get_bit(b.is_strong, x, y) ? 20 : 15;
                }
            }

            //penalty
            score += point_sign(who) * b.delta_points;

            return score;
        }
    };

    namespace Hash {
        //Transposition Table
        int head[TT_HASH_MOD];
        Board status[TT_SIZE];
        int nxt[TT_SIZE];
        int count;

        inline void init() {
            count = 0;
            memset(head, 0, sizeof(head));
        }
        inline int hash(const Board &b) {
            uint64_t hv = 0;
            for(int i = 0; i < sizeof(Board); i++) hv = hv * 131ULL + ((uint8_t *)&b)[i];

            return hv % TT_HASH_MOD;
        }
        inline int ins(const Board &b) {
            int hv = hash(b), i = head[hv];

            while(i) {
                if(0 == memcmp(&status[i], &b, sizeof(Board))) return -1;
                i = nxt[i];
            }

            //boom hash size
            if(count >= TT_SIZE - 1) return -1;

            count++;
            status[count] = b;

            //Link object to chain
            nxt[count] = head[hv];
            head[hv] = count;

            //return id
            return count;
        }
    };

    namespace Search {
        struct State {
            int tt_id;
            int depth;

            int tot_step;
            int tot_time;

            int from_queue_id;
            uint8_t decision_type;
        };

        State Q[TT_SIZE];
        int qH, qT;

        /*
            EVAL = 

             + Time(s)
             + Steps

             + 50 * TowersOccupied
             + 15  * TowerDeltaSum

             + 20 * GridStrongOccupied
             + 10  * GridOccupied

             // + (2*)10 * 8con Me Grids Beside opp Tower Zone //experimental

             - 2 * Penalty
        */
        inline int evaluate(bool who, Board &b, int time_left, int step_left) {
            int score = 0;

            score += step_left; //step
            score += time_left / 1000; //time
            score += 2 * point_sign(who) * b.delta_points; //penalty

            //calculate grid occupy
            for(int x = 0; x < MAX_X; x++) for(int y = 0; y < MAX_Y; y++) if(CASTLE_ID[x][y] == -1) {
                int8_t occupy_status = b.board_state(x, y);

                if(occupy_status != -1) {
                    int cell_score = get_bit(b.is_strong, x, y) ? 20 : 10;

                    score += (occupy_status == who) ? (cell_score) : (-cell_score);
                }
            }

            //calculate castle occupy
            for(int i = 0; i < MAX_CASTLE; i++) if(b.castle[i]) {
                bool castle_owner = (b.castle[i] > 0) ? 0 : 1;

                score += (who == castle_owner) ? 50 : (-50);
                score += 15 * point_sign(who) * VAL_LIMIT(b.castle[i], (int8_t)-8, (int8_t)8);
            }

            return score;
        }

        inline void expand(int from_queue_id, const State& st_from, const Board& new_board, uint8_t decision, int step_cost, int time_cost) {
            int new_tt_id = Hash::ins(new_board);
            if(new_tt_id != -1) {
                Q[qT++] = {
                    new_tt_id,
                    st_from.depth + 1,

                    st_from.tot_step - step_cost,
                    st_from.tot_time - time_cost,

                    from_queue_id,
                    decision
                };
            }
        }

        std::vector<uint8_t> search(bool who, const Board &start, int total_step = GAME_TOTAL_STEP, int total_time = GAME_TOTAL_MS, int time_limit = 3000) {
            //init hash
            Hash::init();

            //init queue
            qH = qT = 0;
            Q[qT++] = {
                Hash::ins(start),
                0,

                total_step,
                total_time,

                -1,
                0
            };

            //record time
            timeval T_start, T_end;
            gettimeofday(&T_start, 0);

            //node count
            int nodes_to_next_tcheck = 0;

            //start search
            while(qH < qT) {
                //check if hash boom
                if(Hash::count >= TT_SIZE - 1) {
                    break;
                }

                //check if tle
                if(++nodes_to_next_tcheck >= 1000000) {
                    nodes_to_next_tcheck = 0;

                    gettimeofday(&T_end, 0);

                    int time_elapsed = (T_end.tv_sec * 1000 + T_end.tv_usec / 1000) - (T_start.tv_sec * 1000 + T_start.tv_usec / 1000);

                    fprintf(stderr, "T=%d Depth=%d\n", time_elapsed, Q[qT - 1].depth);
                    if(time_elapsed >= time_limit) {
                        break;
                    }
                }

                //pick queue head
                int st_queue_id = qH++;
                State &st = Q[st_queue_id];

                //endgame condition
                if(st.tot_step <= 0 || st.tot_time <= 0) {
                    continue;
                }

                Board &b = Hash::status[st.tt_id];
                Board new_board;

                //Expand nodes -- 4 directions
                if(st.tot_step > 0 && st.tot_time >= ACTION_MOVE_MS) {
                    for(int dir = 0; dir < 4; dir++) if(OP::can_move(b, who, dir)) {
                        new_board = b;
                        OP::move(new_board, who, dir);
                        
                        expand(st_queue_id, st, new_board, dir, 1, ACTION_MOVE_MS);
                    }
                }

                //Expand nodes -- occupy
                if(st.tot_time >= ACTION_OCCUPY_MS) {
                    if(OP::can_occupy(b, who)) {
                        new_board = b;
                        OP::occupy(new_board, who);

                        expand(st_queue_id, st, new_board, 4, 0, ACTION_OCCUPY_MS);
                    }
                }

                //Expand nodes -- place ball
                if(st.tot_time >= ACTION_PLACE_BALL_MS) {
                    if(OP::can_place(b, who)) {
                        //place ball
                        if(b.ball[who]) {
                            new_board = b;
                            OP::place(new_board, who, 1);
                            new_board.ball[who]--;

                            expand(st_queue_id, st, new_board, 5, 0, ACTION_PLACE_BALL_MS);
                        }

                        //place cup
                        if(st.tot_time >= ACTION_PLACE_CUP_MS) {
                            if(b.cup[who]) {
                                new_board = b;
                                OP::place(new_board, who, 4);
                                new_board.cup[who]--;

                                expand(st_queue_id, st, new_board, 6, 0, ACTION_PLACE_CUP_MS);
                            }
                        }
                    }
                }
            }

            //use eval func to find best decision
            int max_eval = -2147483648;
            int state_id = 0;
            for(int i = 0; i < qT; i++) {
                int eval = evaluate(who, Hash::status[Q[i].tt_id], Q[i].tot_time, Q[i].tot_step);

                if(eval > max_eval) {
                    max_eval = eval;
                    state_id = i;
                }
            }

            //output best decision
            std::vector<uint8_t> decision;

            int instance = state_id;
            while(instance != 0) {
                decision.push_back(Q[instance].decision_type);
                instance = Q[instance].from_queue_id;
            }
            std::reverse(decision.begin(), decision.end());

            return decision;
        }
    };
};

#endif

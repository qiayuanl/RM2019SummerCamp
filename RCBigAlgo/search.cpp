//ASSET: RED GOES FIRST red=0 blue=1

#include <cstring>
#include <cstdio>
#include <algorithm>
using namespace std;

//Consts
const int MAX_X = 7;
const int MAX_Y = 9;

//Directions
const int dx[4] = {1, 0, -1, 0};
const int dy[4] = {0, 1, 0, -1};

//Bitboard Ops
inline int  position(int x, int y) { return x * MAX_Y + y; }
inline bool get_bit (uint64_t x,   int pos) { return (x >> pos) & 1; }
inline void set_bit_0(uint64_t &x, int pos) { x = x & (~(1ULL << pos)); }
inline void set_bit_1(uint64_t &x, int pos) { x = x | (1ULL << pos); }

struct Board {
    //Real States
    // --board
    uint64_t is_occupy;
    uint64_t who;

    // --castle
    uint8_t castle[8];

    // --position
    uint8_t position_0;
    uint8_t position_1;

    // --carry
    uint8_t ball_0, cup_0;
    uint8_t ball_1, cup_1;

    //Calculated States
    uint64_t is_strong;

    inline void calc_strong() {
        static 
    }
};

int main() {
    Board board;

    printf("%d", sizeof(Board));
}
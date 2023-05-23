#include "evalparams.h"

// params

// inf/mate/draw
const int POS_INF = 100000;
const int NEG_INF = -100000;
const int FAIL_SCORE = -1000000;
const int MATE = 99000;
const int DRAW = 0;
const int MATE_BUFFER = 999;

// pawn values
int PASSED_PAWN = 41;
int UNOBSTRUCTED_PASSER = 59;
int UNSTOPPABLE_PASSED_PAWN = 100;
int CANDIDATE_PASSED_PAWN = 28;
int UNOBSTRUCTED_CANDIDATE = 52;
int BACKWARD_PAWN_PENALTY = 14;
int ISOLATED_PAWN_PENALTY = 27;

// knight values
int KNIGHT_OUTPOST = 6;
int KNIGHT_OUTPOST_ON_HOLE = 6;
int KNIGHT_MOBILITY_MULTIPLIER = 3;
int KNIGHT_MOBILITY_OFFSET = 8;
int KNIGHT_MOBILITY_TABLE[9] = {-7, -1, 1, 3, 5, 6, 7, 8, 9};

// bishop values
int BISHOP_PAIR = 11;
int BISHOP_MOBILITY_MULTIPLIER = 3;
int BISHOP_MOBILITY_OFFSET = 14;
int BISHOP_MOBILITY_TABLE[14] = {-11, 4, 10, 14, 19, 22, 25, 28, 31, 34, 36, 38, 40, 43};

// rook values
int ROOK_OPEN_FILE = 39;
int ROOK_HORIZONTAL_MOBILITY_MULTIPLIER = 2;
int ROOK_HORIZONTAL_MOBILITY_OFFSET = 14;
int ROOK_HORIZONTAL_MOBILITY_TABLE[8] = {-15, -2, 3, 7, 11, 14, 16, 19};
int ROOK_VERTICAL_MOBILITY_MULTIPLIER = 11;
int ROOK_VERTICAL_MOBILITY_OFFSET = 4;
int ROOK_VERTICAL_MOBILITY_TABLE[8] = {0, 14, 19, 24, 28, 31, 34, 37};
int ROOK_CONNECTED = 17;

// king values
int KING_BLOCK_ROOK_PENALTY = 46;
int SAFETY_TABLE_MULTIPLIER = 18;
int SAFETY_TABLE[100] = {0, 0, 0, 1, 3, 5, 7, 10, 14, 17, 22, 26, 31, 37, 43, 49, 56, 63, 71, 79, 88, 97, 106, 116, 126, 137, 148, 160, 172, 185, 198, 211, 225, 239, 254, 269, 285, 301, 317, 334, 352, 369, 388, 406, 425, 445, 465, 485, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500};
int MINOR_ATTACK_UNITS = 2;
int ROOK_ATTACK_UNITS = 3;
int QUEEN_ATTACK_UNITS = 5;
int ROOK_CHECK_UNITS = 4;
int QUEEN_CHECK_UNITS = 7;
int PAWN_SHIELD = 10;
int PAWN_STORM = 8;
int PAWN_SHIELD_DIVISOR = 20;
int PAWN_STORM_DIVISOR = 20;
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
int PASSED_PAWN = 60;
int OUTSIDE_PASSED_PAWN = 20;
int BACKWARD_PAWN_PENALTY = 15;
int ISOLATED_PAWN_PENALTY = 35;

// knight values
int KNIGHT_OUTPOST = 15;
int KNIGHT_OUTPOST_ON_HOLE = 10;

// bishop values
int BISHOP_PAIR = 15;
int BISHOP_MOBILITY = 3;

// rook values
int ROOK_OPEN_FILE = 45;

// king values
int KING_BLOCK_ROOK_PENALTY = 35;
int KING_OPEN_FILE_PENALTY = 61;
int KING_HALF_OPEN_FILE_PENALTY = 40;
int KING_NEXT_TO_OPEN_FILE_PENALTY = 52;
int KING_NEXT_TO_HALF_OPEN_FILE_PENALTY = 35;
int PAWN_SHIELD = 8;
int PAWN_STORM = 6;
int PAWN_SHIELD_DIVISOR = 2000;
int PAWN_STORM_DIVISOR = 2000;
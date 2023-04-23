#include "evalparams.h"

// params

// inf/mate/draw
const int POS_INF = 100000;
const int NEG_INF = -100000;
const int MATE = 99000;
const int DRAW = 0;
const int MATE_BUFFER = 999;

// pawn values
int PASSED_PAWN = 45;
int OUTSIDE_PASSED_PAWN = 9;
int BACKWARD_PAWN_PENALTY = 6;
int ISOLATED_PAWN_PENALTY = 30;

// knight values
int KNIGHT_OUTPOST = 25;
int KNIGHT_OUTPOST_ON_HOLE = 25;

// bishop values
int BISHOP_PAIR = 25;
int BISHOP_MOBILITY = 3;

// rook values
int ROOK_OPEN_FILE = 45;

// king values
int KING_BLOCK_ROOK_PENALTY = 35;
int KING_OPEN_FILE_PENALTY = 61;
int KING_HALF_OPEN_FILE_PENALTY = 60;
int KING_NEXT_TO_OPEN_FILE_PENALTY = 52;
int KING_NEXT_TO_HALF_OPEN_FILE_PENALTY = 45;
int PAWN_SHIELD = 19;
int PAWN_STORM = 18;
int PAWN_SHIELD_DIVISOR = 2001;
int PAWN_STORM_DIVISOR = 2000;
#include "evalparams.h"

// params

// inf/mate/draw
const int POS_INF = 100000;
const int NEG_INF = -100000;
const int MATE = 99000;
const int DRAW = 0;
const int MATE_BUFFER = 999;

// pawn values
int PASSED_PAWN = 60;
int PROTECTED_PASSED_PAWN = 20;
int OUTSIDE_PASSED_PAWN = 20;
int BACKWARD_PAWN_PENALTY = 10;
int ISOLATED_PAWN_PENALTY = 15;
int UNPROTECTED_PAWN_PENALTY = 8;
int DOUBLED_PAWN_PENALTY = 7;

// knight values
int KNIGHT_DECREASE_WITH_PAWN_LOSS = 8;
int KNIGHT_OUTPOST = 10;
int KNIGHT_OUTPOST_ON_HOLE = 10;

// bishop values
int BISHOP_PAIR = 10;
int BISHOP_MOBILITY = 3;

// rook values
int ROOK_INCREASE_WITH_PAWN_LOSS = 8;
int ROOK_OPEN_FILE = 30;
int ROOK_HALF_OPEN_FILE = 18;

// king values
int KING_BLOCK_ROOK_PENALTY = 20;
int KING_OPEN_FILE_PENALTY = 60;
int KING_HALF_OPEN_FILE_PENALTY = 45;
int KING_NEXT_TO_OPEN_FILE_PENALTY = 38;
int KING_NEXT_TO_HALF_OPEN_FILE_PENALTY = 30;
int PAWN_SHIELD = 4;
int PAWN_STORM = 3;
int PAWN_SHIELD_DIVISOR = 2000;

// other scores
int TEMPO_BONUS = 5;
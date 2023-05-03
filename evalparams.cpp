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
int PASSED_PAWN = 50;
int UNSTOPPABLE_PASSED_PAWN = 100;
int CANDIDATE_PASSED_PAWN = 30;
int BACKWARD_PAWN_PENALTY = 22;
int ISOLATED_PAWN_PENALTY = 38;

// knight values
int KNIGHT_OUTPOST = 29;
int KNIGHT_OUTPOST_ON_HOLE = 24;

// bishop values
int BISHOP_PAIR = 29;

// rook values
int ROOK_OPEN_FILE = 40;

// king values
int KING_BLOCK_ROOK_PENALTY = 46;
int KING_OPEN_FILE_PENALTY = 66;
int KING_HALF_OPEN_FILE_PENALTY = 37;
int KING_NEXT_TO_OPEN_FILE_PENALTY = 51;
int KING_NEXT_TO_HALF_OPEN_FILE_PENALTY = 31;
int SAFETY_VERTICAL_SCALE = 500;
int SAFETY_HORIZONTAL_SCALE = 10;
int SAFETY_VERTICAL_SHIFT = 29;
int SAFETY_HORIZONTAL_SHIFT = 40;
int MINOR_ATTACK_UNITS = 2;
int ROOK_ATTACK_UNITS = 3;
int QUEEN_ATTACK_UNITS = 5;
int ROOK_CHECK_UNITS = 3;
int QUEEN_CHECK_UNITS = 6;
int PAWN_SHIELD = 2;
int PAWN_STORM = 6;
int PAWN_SHIELD_DIVISOR = 2000;
int PAWN_STORM_DIVISOR = 1998;

// other values
int TEMPO_BONUS = 25;
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
int PASSED_PAWN = 77;
int UNSTOPPABLE_PASSED_PAWN = 67;
int CANDIDATE_PASSED_PAWN = 36;
int UNOBSTRUCTED_BONUS = 0;
int BACKWARD_PAWN_PENALTY = 141;
int ISOLATED_PAWN_PENALTY = 65;

// knight values
int KNIGHT_OUTPOST = 73;
int KNIGHT_OUTPOST_ON_HOLE = 60;

// rook values
int ROOK_OPEN_FILE = 75;

// king values
int KING_BLOCK_ROOK_PENALTY = 93;
int SAFETY_TABLE_MULTIPLIER = 19;
int MINOR_ATTACK_UNITS = 0;
int ROOK_ATTACK_UNITS = 3;
int QUEEN_ATTACK_UNITS = 8;
int ROOK_CHECK_UNITS = 5;
int QUEEN_CHECK_UNITS = 7;
int PAWN_SHIELD = 17;
int PAWN_STORM = 20;
int PAWN_SHIELD_DIVISOR = 1994;
int PAWN_STORM_DIVISOR = 2003;
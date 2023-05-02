#pragma once 

// params

// inf/mate/draw
extern const int POS_INF;
extern const int NEG_INF;
extern const int FAIL_SCORE;
extern const int MATE;
extern const int DRAW;
extern const int MATE_BUFFER;

// pawn values
extern int PASSED_PAWN;
extern int UNSTOPPABLE_PASSED_PAWN;
extern int CANDIDATE_PASSED_PAWN;
extern int BACKWARD_PAWN_PENALTY;
extern int ISOLATED_PAWN_PENALTY;

// knight values
extern int KNIGHT_OUTPOST;
extern int KNIGHT_OUTPOST_ON_HOLE;

// bishop values
extern int BISHOP_PAIR;

// rook values
extern int ROOK_OPEN_FILE;

// king values
extern int KING_BLOCK_ROOK_PENALTY;
extern int KING_OPEN_FILE_PENALTY;
extern int KING_HALF_OPEN_FILE_PENALTY;
extern int KING_NEXT_TO_OPEN_FILE_PENALTY;
extern int KING_NEXT_TO_HALF_OPEN_FILE_PENALTY;
extern int SAFETY_VERTICAL_SCALE;
extern int SAFETY_HORIZONTAL_SCALE;
extern int SAFETY_VERTICAL_SHIFT;
extern int SAFETY_HORIZONTAL_SHIFT;
extern int MINOR_ATTACK_UNITS;
extern int ROOK_ATTACK_UNITS;
extern int QUEEN_ATTACK_UNITS;
extern int ROOK_CHECK_UNITS;
extern int QUEEN_CHECK_UNITS;
extern int PAWN_SHIELD;
extern int PAWN_STORM;
extern int PAWN_SHIELD_DIVISOR;
extern int PAWN_STORM_DIVISOR;
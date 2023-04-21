#pragma once 

// params

// inf/mate/draw
extern const int POS_INF;
extern const int NEG_INF;
extern const int MATE;
extern const int DRAW;
extern const int MATE_BUFFER;

// pawn values
extern int PASSED_PAWN;
extern int PROTECTED_PASSED_PAWN;
extern int OUTSIDE_PASSED_PAWN;
extern int BACKWARD_PAWN_PENALTY;
extern int ISOLATED_PAWN_PENALTY;
extern int UNPROTECTED_PAWN_PENALTY;
extern int DOUBLED_PAWN_PENALTY;

// knight values
extern int KNIGHT_DECREASE_WITH_PAWN_LOSS;
extern int KNIGHT_OUTPOST;
extern int KNIGHT_OUTPOST_ON_HOLE;

// bishop values
extern int BISHOP_PAIR;
extern int BISHOP_MOBILITY;

// rook values
extern int ROOK_INCREASE_WITH_PAWN_LOSS;
extern int ROOK_OPEN_FILE;
extern int ROOK_HALF_OPEN_FILE;

// king values
extern int KING_BLOCK_ROOK_PENALTY;
extern int KING_OPEN_FILE_PENALTY;
extern int KING_HALF_OPEN_FILE_PENALTY;
extern int KING_NEXT_TO_OPEN_FILE_PENALTY;
extern int KING_NEXT_TO_HALF_OPEN_FILE_PENALTY;
extern int PAWN_SHIELD;
extern int PAWN_STORM;
extern int PAWN_SHIELD_DIVISOR;

// other scores
extern int TEMPO_BONUS;
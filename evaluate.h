#pragma once
#include "game.h"

// masks
constexpr UInt64 SIDES[2] = { 0xFFFFFFFF, 0xFFFFFFFF00000000 };
constexpr UInt64 NEIGHBORING_FILES[8] = {
    0x303030303030303, 0x707070707070707, 0xE0E0E0E0E0E0E0E, 0x1C1C1C1C1C1C1C1C, 0x3838383838383838, 0x7070707070707070, 0xE0E0E0E0E0E0E0E0, 0xC0C0C0C0C0C0C0C0
};
constexpr UInt64 SQUARES_ABOVE_WHITE_PAWNS[8] = {
    0xFFFFFFFFFFFFFF00, 0xFFFFFFFFFFFF0000, 0xFFFFFFFFFF000000, 0xFFFFFFFF00000000, 0xFFFFFF0000000000, 0xFFFF000000000000, 0xFF00000000000000, 0x0
};
constexpr UInt64 SQUARES_BELOW_BLACK_PAWNS[8] = {
    0x0, 0xFF, 0xFFFF, 0xFFFFFF, 0xFFFFFFFF, 0xFFFFFFFFFF, 0xFFFFFFFFFFFF, 0xFFFFFFFFFFFFFF
};

// enum for scores
enum SCORE
{
    // inf/mate/draw
    POS_INF = 100000,
    NEG_INF = -100000,
    MATE = 99000,
    DRAW = 0,
    MATE_BUFFER = 999,

    // pawn values
    PASSED_PAWN = 40,
    PROTECTED_PASSED_PAWN = 15,
    OUTSIDE_PASSED_PAWN = 40,
    UNOBSTRUCTED_PASSED_PAWN = 20,
    ISOLATED_PAWN_PENALTY = 15,
    UNPROTECTED_PAWN_PENALTY = 5,

    // knight values
    KNIGHT_DECREASE_WITH_PAWN_LOSS = 5,
    KNIGHT_OUTPOST = 10,
    KNIGHT_OUTPOST_ON_HOLE_BONUS = 8,

    // bishop values
    BISHOP_PAIR = 10,

    // rook values
    ROOK_INCREASE_WITH_PAWN_LOSS = 4,
    ROOK_OPEN_FILE = 25,
    ROOK_CONTROL_AH_FILE = 15,
    ROOK_HALF_OPEN_FILE = 9,
    ROOK_ON_SEVENTH = 10,
    ROOKS_CONNECTED = 12,

    // king values
    KING_BLOCK_ROOK_PENALTY = 20,
    KING_OPEN_FILE_PENALTY = 50,
    KING_NEXT_TO_OPEN_FILE_PENALTY = 35,
    PAWN_SHIELD = 3,

    // other scores
    TEMPO_BONUS = 5
};

// lazy eval function
int lazyEvaluate(Board& board);

// full eval function
int evaluate(Board& board);

// bishop stuff
bool hasBishopPair(Board& board, Color color);

// knight stuff
bool isKnightOutpost(Board& board, Color color, Square square);
bool isKnightOnHole(Board& board, Color color, Square square);
int knightOutpostScore(Board& board, Color color);

// rook stuff
void rookScore(Board& board, Color color, int& openingScore, int& endgameScore);
bool kingBlockRook(Board& board, Color color, Square rookSquare);

// pawn stuff
bool isPassedPawn(Board& board, Color color, Square square);
bool pawnIsObstructed(Board& board, Color color, Square square);
void pawnScore(Board& board, Color color, int& openingScore, int& endgameScore);

// king stuff
int kingPawnShieldScore(Board& board, Color color, Square square);
void kingScore(Board& board, Color color, int& openingScore, int& endgameScore);

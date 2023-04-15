#pragma once
#include "game.h"

// masks
constexpr UInt64 SIDES[2] = { 0xFFFFFFFF, 0xFFFFFFFF00000000 };

// enum for scores
enum SCORE
{
    // inf/mate/draw
    POS_INF = 100000,
    NEG_INF = -100000,
    MATE = 99000,
    DRAW = 0,
    MATE_BUFFER = 999,

    // knight values
    KNIGHT_DECREASE_WITH_PAWN_LOSS = 10,
    KNIGHT_OUTPOST = 10,
    KNIGHT_OUTPOST_ON_HOLE_BONUS = 8,

    // bishop values
    BISHOP_PAIR = 15,

    // rook values
    ROOK_INCREASE_WITH_PAWN_LOSS = 10,

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


#pragma once
#include "game.h"
#include "pawnhash.h"

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
    PASSED_PAWN = 60,
    PROTECTED_PASSED_PAWN = 20,
    OUTSIDE_PASSED_PAWN = 20,
    BACKWARD_PAWN_PENALTY = 10,
    ISOLATED_PAWN_PENALTY = 15,
    UNPROTECTED_PAWN_PENALTY = 8,
    DOUBLED_PAWN_PENALTY = 7,

    // knight values
    KNIGHT_DECREASE_WITH_PAWN_LOSS = 8,
    KNIGHT_OUTPOST = 10,
    KNIGHT_OUTPOST_ON_HOLE = 10,

    // bishop values
    BISHOP_PAIR = 10,

    // rook values
    ROOK_INCREASE_WITH_PAWN_LOSS = 8,
    ROOK_OPEN_FILE = 30,
    ROOK_HALF_OPEN_FILE = 18,

    // king values
    KING_BLOCK_ROOK_PENALTY = 20,
    KING_OPEN_FILE_PENALTY = 60,
    KING_HALF_OPEN_FILE_PENALTY = 45,
    KING_NEXT_TO_OPEN_FILE_PENALTY = 38,
    KING_NEXT_TO_HALF_OPEN_FILE_PENALTY = 30,
    PAWN_SHIELD = 4,
    PAWN_STORM = 3,
    PAWN_SHIELD_DIVISOR = 2000,

    // other scores
    TEMPO_BONUS = 5
};

// lazy eval function
int lazyEvaluate(Board& board);

// full eval function
int evaluate(Board& board, PawnTable* pawnTable);

// bishop stuff
bool hasBishopPair(Board& board, Color color);

// knight stuff
bool isHole(Board& board, Color color, Square square);
bool isKnightOutpost(Board& board, Color color, Square square);
int knightOutpostScore(Board& board, Color color);

// rook stuff
bool openFile(Board& board, Color color, int file);
bool halfOpenFile(Board& board, Color color, int file);
bool kingBlockRook(Board& board, Color color, Square rookSquare);
void rookScore(Board& board, Color color, int& openingScore, int& endgameScore);

// pawn stuff
bool isPassed(Board& board, Color color, Square square);
bool isProtected(Board& board, Color color, Square square);
bool isOutside(Board& board, Color color, Square square);
bool isIsolated(Board& board, Color color, Square square);
bool isBackward(Board& board, Color color, Square square);
bool isDoubled(Board& board, Color color, Square square);
void pawnScore(Board& board, Color color, int& openingScore, int& endgameScore);

// king stuff
UInt64 kingSafetyArea(Color color, Square square);
UInt64 kingDangerArea(Color color, Square square);
int kingPawnShieldScore(Board& board, Color color, Square square);
int kingPawnStormScore(Board& board, Color color, Square square);
void kingScore(Board& board, Color color, int& openingScore, int& endgameScore);

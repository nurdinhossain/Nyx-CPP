#pragma once
#include "game.h"
#include "transposition.h"

const int MVV_LVA[6][6] = 
{
    {15, 14, 13, 12, 11, 10}, // victim P, attacker P, N, B, R, Q
    {25, 24, 23, 22, 21, 20}, // victim N, attacker P, N, B, R, Q
    {35, 34, 33, 32, 31, 30}, // victim B, attacker P, N, B, R, Q
    {45, 44, 43, 42, 41, 40}, // victim R, attacker P, N, B, R, Q
    {55, 54, 53, 52, 51, 50}, // victim Q, attacker P, N, B, R, Q
    {0, 0, 0, 0, 0, 0}  // victim K, attacker P, N, B, R, Q
};

const int TT_MOVE = 9999999;
const int PROMO_OFFSET = 999999;
const int CAPTURE_OFFSET = 99999;
const int PAWN_NEAR_PROMO_OFFSET = 9999;
const int KNIGHT_CENTER = 1000;
const int OUTPOST = 1500;
const int OUTPOST_ON_HOLE = 500;
const int KILLER_VALUE = 10;
const int HISTORY_MULTIPLIER = 500;

// score moves based on MVV/LVA
void scoreMoves(Board& board, TranspositionTable* tt, Move killerMoves[][2], Move moves[], int historyTable[2][64][64], int historyMax, int numMoves, int ply);

// sort moves based on score
void sortMoves(Move moves[], int numMoves);

// randomize move order
void randomizeMoves(Move moves[], int numMoves);
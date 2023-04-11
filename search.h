#pragma once
#include "game.h"

// constants
const int MAX_DEPTH = 64;
const int MAX_MOVES = 256;
const int MAX_MOVES_ATTACK = 32;
const int MAX_TIME = 7;

// alpha beta search negamax
int search(Board board, int depth, int ply, int alpha, int beta, Move &bestMoveCurrentIteration, int &bestScoreCurrentIteration, auto start);

// quiescence search
int quiesce(Board board, int alpha, int beta);

// iterative deepening
Move getBestMove(Board board);
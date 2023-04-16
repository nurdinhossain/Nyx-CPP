#pragma once
#include "game.h"

// constants

// LMR
const int LMR_LIMIT = 4;

// Futility pruning
const int FUTILE_MAX_DEPTH = 2;
const int FUTILE_MARGINS[] = { 0, 150, 350 };
const int FUTILE_MARGIN_Q = 100;

// Razoring
const int RAZOR_MARGIN = 200;
const int RAZOR_DEPTH = 3;

// Reverse futility pruning
const int REVERSE_FUTILE_MAX_DEPTH = 2;
const int REVERSE_FUTILE_MARGINS[] = { 0, 250, 450 };

// Null move pruning
const int MAX_R = 4;
const int MIN_R = 3;
const int DR = 4; // depth reduction value
const int NULL_DEPTH = 4;

// methods for boosting search
bool moveCausesCheck(Board& board, Move move);
bool lmrValid(Board& board, Move move, int moveIndex, int depth);
int lmrReduction(int moveIndex, int depth);
bool futile(Board& board, Move move, int moveIndex, int depth, int alpha, int beta);
bool razorOk(Board& board, int depth, int alpha);
bool reverseFutileOk(Board& board, int depth,  int beta);
int see(Board& board, Square initialFrom, Square initialTo);
bool nullOk(Board& board, int depth);
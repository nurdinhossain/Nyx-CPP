#include "game.h"
#include <vector>

// constants
const int LMR_LIMIT = 4;
const int FUTILE_MAX_DEPTH = 2;
const int FUTILE_MARGINS[] = { 0, 250, 500 };

// methods for boosting search
bool lmrValid(Board& board, Move move, int moveIndex, int depth);
int lmrReduction(int moveIndex, int depth);
bool futile(Board& board, Move move, int moveIndex, int depth, int alpha, int beta);
int see(Board& board, Square initialFrom, Square initialTo);
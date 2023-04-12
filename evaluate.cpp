#include "evaluate.h"
#include <iostream>

// eval function
int evaluate(Board board)
{
    int phase = board.getPhase();

    // material
    int openingScore = board.getMaterial(WHITE) - board.getMaterial(BLACK);
    int endgameScore = openingScore;

    // piece square tables
    openingScore += board.getStaticEvalOpening();
    endgameScore += board.getStaticEvalEndgame();

    // interpolate between opening and endgame
    int score = (openingScore * (256 - phase) + endgameScore * phase) / 256;

    // return the score based on the side to move
    return score * (board.getNextMove() == WHITE ? 1 : -1);
}
#pragma once
#include "game.h"

// enum for scores
enum SCORE
{
    POS_INF = 100000,
    NEG_INF = -100000,
    MATE = 99000,
    DRAW = 0,
    MATE_BUFFER = 999
};

// eval function
int evaluate(Board board);
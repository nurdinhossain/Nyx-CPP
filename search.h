#pragma once
#include "game.h"
#include "transposition.h"

// constants
const int MAX_DEPTH = 64;
const int MAX_MOVES = 256;
const int MAX_MOVES_ATTACK = 64;
const int MAX_TIME = 5;

// struct for gathering statistics about the search
struct SearchStats
{
    int nodes;
    int qNodes;
    int cutoffs;
    int qCutoffs;
    int ttHits;

    // print and clear methods
    void print();
    void clear();
};

// AI class
class AI
{
    public:
        // constructor/destructor
        AI();
        ~AI();

        // search methods
        int search(Board& board, int depth, int ply, int alpha, int beta, auto start);
        int quiesce(Board& board, int alpha, int beta);
        Move getBestMove(Board& board);

    private:
        // private fields
        TranspositionTable* transpositionTable_;
        Move bestMoveCurrentIteration_;
        int bestScoreCurrentIteration_;
        SearchStats searchStats_;
};
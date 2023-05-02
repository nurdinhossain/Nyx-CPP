#pragma once
#include <chrono>
#include "game.h"
#include "transposition.h"
#include "pawnhash.h"

// constants
const int MAX_DEPTH = 64;
const int MAX_MOVES = 256;
const int MAX_MOVES_ATTACK = 64;
const int MAX_TIME = 20;
const int KILLER_MAX_PLY = 64;
const int THREADS = 3;

// aspiration window    
const int ASPIRATION_WINDOW[6] = { 25, 150, 600, 100000 };

// struct for gathering statistics about the search
struct SearchStats
{
    int nodes{0};
    int qNodes{0};
    int cutoffs{0};
    int qCutoffs{0};
    int ttHits{0};
    int killersStored{0};
    int reSearches{0};
    int lmrReductions{0};
    int lmpPruned{0};
    int futileReductions{0};
    int futileReductionsQ{0};
    int deltaPruned{0};
    int seePruned{0};
    int nullReductions{0};
    int reverseFutilePruned{0};
    int razorPruned{0};
    int multiCutPruned{0};
    int extensions{0};
    int iidHits{0};

    // print and clear methods
    void print();
    void clear();
};

// AI class
class AI
{
    public:
        // public fields
        Move killerMoves_[KILLER_MAX_PLY][2] = {Move()};
        int historyTable_[2][64][64] = {0};
        int historyMax_ = 1;

        // constructor/destructor
        AI();
        AI(bool depthPreferred);
        ~AI();

        // search methods
        int search(Board& board, TranspositionTable* transpositionTable_, int depth, int ply, int alpha, int beta, bool cut, std::chrono::steady_clock::time_point start);
        Move getBestMove(Board& board, TranspositionTable* transpositionTable_, int increment, bool verbose);
        int quiesce(Board& board, int alpha, int beta);

        // history table methods
        void ageHistory();

        // getters
        SearchStats& getSearchStats() { return searchStats_; }

    private:
        // private fields
        PawnTable* pawnTable_;
        Move bestMoveCurrentIteration_;
        SearchStats searchStats_;
        bool depthPreferred_;
};

// print pv line
void printPV(Board& board, TranspositionTable* transpositionTable_);

// threaded search method
Move threadedSearch(AI& master, Board& board, TranspositionTable* transpositionTable_);
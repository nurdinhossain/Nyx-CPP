#pragma once
#include "game.h"

// enum for flags
enum Flag
{
    EXACT,
    LOWER_BOUND,
    UPPER_BOUND
};

// struct for storing transposition table entries
struct Entry
{
    UInt64 key;
    Flag flag;
    int depth;
    int score;
    Move move;
};

// class for transposition table
class TranspositionTable
{
    public:
        // constructor and destructor
        TranspositionTable(int size);
        ~TranspositionTable();

        // helpers/getters
        void clear();
        int size();
        Move getMove(UInt64 key);
        int correctScoreStore(int score, int ply);
        int correctScoreRead(int score, int ply);

        // store/access
        void store(UInt64 key, Flag flag, int depth, int ply, int score, Move move);
        Entry* probe(UInt64 key);
    private:
        // variables
        int size_;
        Entry* table_;
};
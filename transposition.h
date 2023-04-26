#pragma once
#include "game.h"

// constants
const int TT_SIZE = 64; // in MB

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
    UInt64 key{0ULL};
    Flag flag{EXACT};
    int depth{0};
    int score{0};
    Move move{Move()};
};

// class for transposition table
class TranspositionTable
{
    public:
        // constructor and destructor
        TranspositionTable();
        TranspositionTable(int mb);
        ~TranspositionTable();

        // helpers/getters
        void clear();
        Move getMove(UInt64 key);
        int correctScoreStore(int score, int ply);
        int correctScoreRead(int score, int ply);
        bool verifyEntry(UInt64 key, Entry* entry);

        // store/access
        void store(UInt64 key, Flag flag, int depth, int ply, int score, Move move);
        Entry* probe(UInt64 key);
        int getScore(UInt64 key, int depth, int ply, int alpha, int beta);
    private:
        // variables
        int size_;
        Entry* table_;
};
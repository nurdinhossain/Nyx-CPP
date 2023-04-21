#include "transposition.h"
#include "evaluate.h"
#include "evalparams.h"
#include <iostream>
#include <cmath>

// constructor and destructor
TranspositionTable::TranspositionTable()
{
    // ensure size is a power of 2
    int tt_size = 1 << (int)log2(TT_SIZE);

    // set size
    size_ = tt_size * 1024 * 1024 / sizeof(Entry);

    // initialize array
    table_ = new Entry[size_];

    std::cout << "Transposition table size: " << size_ << std::endl;
}

TranspositionTable::TranspositionTable(int mb)
{
    // ensure size is a power of 2
    mb = 1 << (int)log2(mb);

    // set size
    size_ = mb * 1024 * 1024 / sizeof(Entry);

    // initialize array
    table_ = new Entry[size_];

    std::cout << "Transposition table size: " << size_ << std::endl;
}

TranspositionTable::~TranspositionTable()
{
    // delete the table
    delete[] table_;

    std::cout << "Transposition table deleted" << std::endl;
}

// helpers/getters 

void TranspositionTable::clear()
{
    std::cout << "Clearing transposition table" << std::endl;
    // clear the table
    for (int i = 0; i < size_; i++)
    {
        table_[i].key = 0;
        table_[i].flag = EXACT;
        table_[i].depth = 0;
        table_[i].score = 0;
        table_[i].move = Move();
    }

    std::cout << "Transposition table cleared" << std::endl;
}

Move TranspositionTable::getMove(UInt64 key)
{
    // get the entry
    Entry* entry = probe(key);

    // return the move
    return entry->move;
}

int TranspositionTable::correctScoreStore(int score, int ply)
{
    // if the score is a mate score, adjust it
    if (score >= MATE - MATE_BUFFER)
    {
        score += ply;
    }
    else if (score <= -MATE + MATE_BUFFER)
    {
        score -= ply;
    }

    // return the score
    return score;
}

int TranspositionTable::correctScoreRead(int score, int ply)
{
    // if the score is a mate score, adjust it
    if (score >= MATE - MATE_BUFFER)
    {
        score -= ply;
    }
    else if (score <= -MATE + MATE_BUFFER)
    {
        score += ply;
    }

    // return the score
    return score;
}

// store/access
Entry* TranspositionTable::probe(UInt64 key)
{
    // get the index
    int index = key % size_;

    // return the entry
    return &table_[index];
}

void TranspositionTable::store(UInt64 key, Flag flag, int depth, int ply, int score, Move move)
{
    // get the entry
    Entry* entry = probe(key);

    // store the data
    entry->key = key;
    entry->flag = flag;
    entry->depth = depth;
    entry->score = correctScoreStore(score, ply);
    entry->move = move;
}

int TranspositionTable::getScore(UInt64 key, int depth, int ply, int alpha, int beta)
{
    // get the entry
    Entry* entry = probe(key);

    // ensure the key matches
    if (entry->key != key)
    {
        return NEG_INF;
    }

    // if key does match, check if the depth is sufficient
    if (entry->depth >= depth)
    {
        // get the score
        int score = correctScoreRead(entry->score, ply);

        // check the flag
        if (entry->flag == EXACT)
        {
            return score;
        }
        else if (entry->flag == UPPER_BOUND)
        {
            if (score <= alpha)
            {
                return score;
            }
        }
        else if (entry->flag == LOWER_BOUND)
        {
            if (score >= beta)
            {
                return score;
            }
        }
    }

    // return negative infinity
    return NEG_INF;
}
#include "transposition.h"
#include "evaluate.h"
#include <iostream>
#include <cmath>
#include <cstring>

// constructor and destructor
TranspositionTable::TranspositionTable()
{
    setSize(0);
}

TranspositionTable::TranspositionTable(int mb)
{
    setSize(mb);
}

TranspositionTable::~TranspositionTable()
{
    free(table_);
}

// helpers/getters
void TranspositionTable::setSize(int mb)
{
    // get a power of 2 for the size
    mb = 1 << (int)log2(mb);

    // set the size
    size_ = (mb * 1024 * 1024) / sizeof(Entry); // 1 MB = 1024 KB = 1024 * 1024 B

    // free the table if it exists
    if (table_ != NULL)
    {
        free(table_);
    }

    // allocate the table
    table_ = (Entry*)calloc(size_, sizeof(Entry));
}   

void TranspositionTable::clear()
{
    // clear the table using memset
    memset(table_, 0, size_ * sizeof(Entry));
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
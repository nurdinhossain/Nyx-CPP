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
}

TranspositionTable::~TranspositionTable()
{
    // delete the table
    delete[] table_;
}

// helpers/getters 

void TranspositionTable::clear()
{
    // clear the table
    for (int i = 0; i < size_; i++)
    {
        table_[i].key = 0;
        table_[i].flag = EXACT;
        table_[i].depth = 0;
        table_[i].score = 0;
        table_[i].move = Move();
    }
}

Move TranspositionTable::getMove(UInt64 key)
{
    // get the entry
    Entry* entry = probe(key);

    // ensure the key matches
    if (!verifyEntry(key, entry))
    {
        return Move();
    }

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

bool TranspositionTable::verifyEntry(UInt64 key, Entry* entry)
{
    UInt64 entryKey = entry->key;

    // unwrap all the xor'd components
    Move move = entry->move;

    // move.to
    entryKey ^= move.to;

    // move.from
    entryKey ^= ((UInt64)move.from << 6);

    // move.type
    entryKey ^= ((UInt64)move.type << 12);

    // score
    entryKey ^= ((UInt64)entry->score << 16);

    // depth
    entryKey ^= ((UInt64)entry->depth << 34);

    // flag
    entryKey ^= ((UInt64)entry->flag << 40);

    // check if entryKey matches key
    return entryKey == key;
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
    // if score is a fail score, don't store it
    if (abs(score) == abs(FAIL_SCORE))
    {
        return;
    }

    // get the entry
    Entry* entry = probe(key);

    // store the data in a thread-safe manner
    UInt64 newKey = key;
    int correctScore = correctScoreStore(score, ply);

    // first hash move
    Square from = move.from, to = move.to;
    MoveType type = move.type;
    newKey ^= to; 
    newKey ^= ((UInt64)from << 6);
    newKey ^= ((UInt64)type << 12);
    
    // then hash score
    newKey ^= ((UInt64)correctScore << 16);

    // then hash depth
    newKey ^= ((UInt64)depth << 34);

    // then hash flag
    newKey ^= ((UInt64)flag << 40);

    // store the data
    entry->key = newKey;
    entry->flag = flag;
    entry->depth = depth;
    entry->score = correctScore;
    entry->move = move;
}

int TranspositionTable::getScore(UInt64 key, int depth, int ply, int alpha, int beta)
{
    // get the entry
    Entry* entry = probe(key);

    // ensure the key matches
    if (!verifyEntry(key, entry))
    {
        return FAIL_SCORE;
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
    return FAIL_SCORE;
}
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

    // clear the table
    clear();

    // print size
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

    // clear the table
    clear();

    // print size
    std::cout << "Transposition table size: " << size_ << std::endl;
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
        table_[i].smpKey = 0;
        table_[i].data = 0;
    }
}

Move TranspositionTable::getMove(UInt64 key)
{
    // get the entry
    Entry* entry = probe(key);

    // get key and data
    UInt64 smpKey = entry->smpKey;
    UInt64 data = entry->data;

    // check if the entry is valid
    if (key != (smpKey ^ data))
    {
        return Move();
    }

    // extract move from data
    Square to = static_cast<Square>(data & 0x3F);
    Square from = static_cast<Square>((data >> 6) & 0x3F);
    MoveType type = static_cast<MoveType>((data >> 12) & 0xF);
    Move move = {type, from, to};

    // return the move
    return move;
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

void TranspositionTable::store(UInt64 key, Flag flag, int depth, int ply, int score, Move move, bool depthPreferred)
{
    // if score is a fail score, don't store it
    if (abs(score) == abs(FAIL_SCORE))
    {
        return;
    }

    // get the entry
    Entry* entry = probe(key);

    // only replace if the depth is greater than the current depth and the depth is preferred
    if (depthPreferred)
    {
        if (depth < ((entry->data >> 34) & 0x3F))
        {
            return;
        }
    }

    // store the data in a thread-safe manner
    UInt64 smpKey = key;
    UInt64 data = 0ULL;
    int correctScore = correctScoreStore(score, ply);

    // first hash move
    Square from = move.from, to = move.to;
    MoveType type = move.type;
    data ^= to; 
    data ^= ((UInt64)from << 6);
    data ^= ((UInt64)type << 12);
    
    // then hash score
    data ^= ((UInt64)(correctScore + POS_INF) << 16); // offset by POS_INF to avoid negative numbers

    // then hash depth
    data ^= ((UInt64)depth << 34);

    // then hash flag
    data ^= ((UInt64)flag << 40);

    // xor the data with the key
    smpKey ^= data;

    // store the data
    entry->smpKey = smpKey;
    entry->data = data;
}

int TranspositionTable::getScore(UInt64 key, int depth, int ply, int alpha, int beta, bool pvNode)
{
    // get the entry
    Entry* entry = probe(key);

    // get key and data
    UInt64 smpKey = entry->smpKey;
    UInt64 data = entry->data;

    // check if the entry is valid
    if (key != (smpKey ^ data))
    {
        return FAIL_SCORE;
    }

    // get depth, score, and flag
    int entryDepth = (data >> 34) & 0x3F;
    int entryScore = (data >> 16) & 0x3FFFF;
    Flag entryFlag = static_cast<Flag>((data >> 40) & 3);

    // if key does match, check if the depth is sufficient
    if (entryDepth >= depth)
    {
        // get the score
        int score = correctScoreRead(entryScore - POS_INF, ply);

        // check the flag
        if (entryFlag == EXACT)
        {
            return score;
        }
        else if (entryFlag == UPPER_BOUND)
        {
            if (score <= alpha)
            {
                return score;
            }
        }
        else if (entryFlag == LOWER_BOUND)
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
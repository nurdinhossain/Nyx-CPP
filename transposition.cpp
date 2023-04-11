#include "transposition.h"
#include "evaluate.h"

// constructor and destructor
TranspositionTable::TranspositionTable(int size)
{
    // set size
    size_ = size;

    // allocate memory
    table_ = new Entry[size_];
}

TranspositionTable::~TranspositionTable()
{
    // free memory
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

int TranspositionTable::size()
{
    return size_;
}

Move TranspositionTable::getMove(UInt64 key)
{
    // get the entry
    Entry* entry = probe(key);

    // if the entry exists, return the move
    if (entry != nullptr)
    {
        return entry->move;
    }

    // otherwise, return a null move
    return Move();
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
void TranspositionTable::store(UInt64 key, Flag flag, int depth, int ply, int score, Move move)
{
    // get the entry
    Entry* entry = probe(key);

    // if the entry exists, store the data
    if (entry != nullptr)
    {
        entry->key = key;
        entry->flag = flag;
        entry->depth = depth;
        entry->score = correctScoreStore(score, ply);
        entry->move = move;
    }
}

Entry* TranspositionTable::probe(UInt64 key)
{
    // get the index
    int index = key % size_;

    // if the key matches, return the entry
    if (table_[index].key == key)
    {
        return &table_[index];
    }

    // otherwise, return null
    return nullptr;
}
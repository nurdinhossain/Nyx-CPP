#include <iostream>
#include <chrono>
#include "search.h"
#include "evaluate.h"
#include "moveorder.h"

// implement struct member functions
void SearchStats::print()
{
    std::cout << "Nodes: " << nodes;
    std::cout << ", QNodes: " << qNodes;
    std::cout << ", Cutoffs: " << cutoffs;
    std::cout << ", QCutoffs: " << qCutoffs << std::endl;
}

void SearchStats::clear()
{
    nodes = 0;
    qNodes = 0;
    cutoffs = 0;
    qCutoffs = 0;
}

/* IMPLEMENT AI METHODS */

// constructor/destructor
AI::AI()
{
    // initialize transposition table
    transpositionTable_ = TranspositionTable(TT_SIZE);

    // initialize search stats
    searchStats_ = SearchStats();
}

AI::~AI()
{
    // delete transposition table
    delete &transpositionTable_; 
}

// search
int AI::search(Board board, int depth, int ply, int alpha, int beta, auto start)
{
    // check for time
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;
    if (elapsed.count() >= MAX_TIME)
    {
        return NEG_INF;
    }

    // increment nodes
    searchStats_.nodes++;

    // insufficient material
    if (board.insufficientMaterial(WHITE) && board.insufficientMaterial(BLACK))
    {
        return DRAW;
    }

    // check for threefold repetition
    if (board.getHistory() > 1)
    {
        return DRAW;
    }

    // check for max depth
    if (depth <= 0)
    {
        return quiesce(board, alpha, beta);
    }

    // generate moves
    board.moveGenerationSetup();
    Move moves[MAX_MOVES];
    int numMoves = board.generateMoves(moves);

    // check for mate/stalemate
    if (numMoves == 0)
    {
        if (board.getCheckers() != 0)
        {
            return -MATE + ply;
        }
        else
        {
            return DRAW;
        }
    }

    // sort moves 
    scoreMoves(board, moves, numMoves);
    sortMoves(moves, numMoves);

    // loop through moves
    for (int i = 0; i < numMoves; i++)
    {
        // make move
        board.makeMove(moves[i]);

        // search
        int score = -search(board, depth - 1, ply + 1, -beta, -alpha, start);

        // undo move
        board.unmakeMove(moves[i]);

        // check for cutoff
        if (score >= beta)
        {
            searchStats_.cutoffs++;
            return beta;
        }

        // update alpha
        if (score > alpha)
        {
            alpha = score;

            if (ply == 0)
            {
                bestMoveCurrentIteration_ = moves[i];
                bestScoreCurrentIteration_ = score;
            }
        }
    }

    // return alpha
    return alpha;
}

// quiescence search
int AI::quiesce(Board board, int alpha, int beta)
{
    // increment qNodes
    searchStats_.qNodes++;

    // evaluate board
    int score = evaluate(board);

    // check for cutoff
    if (score >= beta)
    {
        searchStats_.qCutoffs++;
        return beta;
    }

    // update alpha
    if (score > alpha)
    {
        alpha = score;
    }

    // generate attacking moves
    board.moveGenerationSetup();
    Move moves[MAX_MOVES_ATTACK];
    int numMoves = board.generateMoves(moves, true);

    // sort moves
    scoreMoves(board, moves, numMoves);
    sortMoves(moves, numMoves);

    // loop through moves
    for (int i = 0; i < numMoves; i++)
    {
        // make move
        board.makeMove(moves[i]);

        // search
        score = -quiesce(board, -beta, -alpha);

        // undo move
        board.unmakeMove(moves[i]);

        // check for cutoff
        if (score >= beta)
        {
            searchStats_.qCutoffs++;
            return beta;
        }

        // update alpha
        if (score > alpha)
        {
            alpha = score;
        }
    }

    // return alpha
    return alpha;
}

// get best move through iterative deepening
Move AI::getBestMove(Board board)
{
    // initialize variables
    Move bestMove = Move();
    int bestScore = 0;
    int depth = 1;
    
    // iterative deepening with time
    auto start = std::chrono::high_resolution_clock::now();
    while (depth <= MAX_DEPTH)
    {
        // clear stats
        searchStats_.clear();

        // search
        search(board, depth, 0, NEG_INF, POS_INF, start);

        // check for time
        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> elapsed = end - start;
        if (elapsed.count() >= MAX_TIME)
        {
            break;
        }

        // update best move and score if the call was not broken prematurely
        bestMove = bestMoveCurrentIteration_;
        bestScore = bestScoreCurrentIteration_;  

        // print info
        std::cout << "depth: " << depth;
        std::cout << ", time: " << elapsed.count();
        std::cout << ", best move: " << indexToSquare(bestMove.from) << indexToSquare(bestMove.to);
        std::cout << ", score: " << bestScore << ", ";
        searchStats_.print();

        // check for mate
        if (bestScore >= MATE - MAX_DEPTH || bestScore <= -MATE + MAX_DEPTH)
        {
            break;
        }

        // increment depth
        depth++; 
    }

    // return best move
    return bestMove;
}
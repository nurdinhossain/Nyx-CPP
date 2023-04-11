#include <iostream>
#include "search.h"
#include "evaluate.h"
#include <chrono>

// alpha beta search negamax
int search(Board board, int depth, int ply, int alpha, int beta, Move &bestMoveCurrentIteration, int &bestScoreCurrentIteration, auto start)
{
    // check for time
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;
    if (elapsed.count() >= MAX_TIME)
    {
        return NEG_INF;
    }

    // check for max depth
    if (depth <= 0)
    {
        int value = quiesce(board, alpha, beta);
        return value;
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

    if (board.insufficientMaterial(WHITE) && board.insufficientMaterial(BLACK))
    {
        return DRAW;
    }

    // sort moves *NOT IMPLEMENTED*

    // loop through moves
    for (int i = 0; i < numMoves; i++)
    {
        // make move
        board.makeMove(moves[i]);

        // search
        int score = -search(board, depth - 1, ply + 1, -beta, -alpha, bestMoveCurrentIteration, bestScoreCurrentIteration, start);

        // unmake move
        board.unmakeMove(moves[i]);

        // check for beta cutoff
        if (score >= beta)
        {
            return beta;
        }   

        // check for new best move and update alpha
        if (score > alpha)
        {
            // update alpha
            alpha = score;

            // update best move
            if (ply == 0)
            {
                bestMoveCurrentIteration = moves[i];
                bestScoreCurrentIteration = score;
            }
        }
    }

    // return alpha
    return alpha;
}

// quiescence search
int quiesce(Board board, int alpha, int beta)
{   
    // evaluate board
    int score = evaluate(board);

    // check for beta cutoff
    if (score >= beta)
    {
        return beta;
    }

    // check for new best move and update alpha
    if (score > alpha)
    {
        // update alpha
        alpha = score;
    }

    // generate attacking moves
    board.moveGenerationSetup();
    Move moves[MAX_MOVES];
    int numMoves = board.generateMoves(moves, true);

    // sort moves *NOT IMPLEMENTED*

    // loop through moves
    for (int i = 0; i < numMoves; i++)
    {
        // make move
        board.makeMove(moves[i]);

        // search
        score = -quiesce(board, -beta, -alpha);

        // unmake move
        board.unmakeMove(moves[i]);

        // check for beta cutoff
        if (score >= beta)
        {
            return beta;
        }

        // check for new best move and update alpha
        if (score > alpha)
        {
            // update alpha
            alpha = score;
        }
    }

    // return alpha
    return alpha;
}

// iterative deepening
Move getBestMove(Board board)
{
    // initialize variables
    Move bestMove = Move();
    int bestScore = 0;
    int depth = 1;

    // iterative deepening with time
    auto start = std::chrono::high_resolution_clock::now();
    while (depth <= MAX_DEPTH)
    {
        // search
        int score = search(board, depth, 0, NEG_INF, POS_INF, bestMove, bestScore, start);

        // check for time
        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> elapsed = end - start;
        if (elapsed.count() >= MAX_TIME)
        {
            break;
        }

        // check for mate
        if (score >= MATE - MAX_DEPTH || score <= -MATE + MAX_DEPTH)
        {
            break;
        }

        // increment depth
        depth++;

        // print info
        std::cout << "depth: " << depth << std::endl;
    }

    // print info
    std::cout << "best move: " << indexToSquare(bestMove.from) << indexToSquare(bestMove.to) << std::endl;
    std::cout << "best score: " << bestScore << std::endl;
    std::cout << "depth: " << depth << std::endl;

    // return best move
    return bestMove;
}
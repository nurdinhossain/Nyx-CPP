#include <iostream>
#include <chrono>
#include <thread>
#include <cmath>
#include "search.h"
#include "evaluate.h"
#include "moveorder.h"
#include "searchboost.h"
#include "evalparams.h"

// implement struct member functions
void SearchStats::print()
{
    std::cout << "Nodes: " << nodes;
    std::cout << ", QNodes: " << qNodes;
    std::cout << ", Cutoffs: " << cutoffs;
    std::cout << ", QCutoffs: " << qCutoffs;
    std::cout << ", TT Hits: " << ttHits;
    std::cout << ", Killers Stored: " << killersStored;
    std::cout << ", ReSearches: " << reSearches;
    std::cout << ", LMR Reductions: " << lmrReductions;
    std::cout << ", LMP Pruned: " << lmpPruned;
    std::cout << ", Futile Reductions: " << futileReductions;
    std::cout << ", Futile Reductions Q: " << futileReductionsQ;
    std::cout << ", Delta Pruned: " << deltaPruned;
    std::cout << ", SEE Pruned: " << seePruned;
    std::cout << ", Null Reductions: " << nullReductions;
    std::cout << ", Reverse Futile Pruned: " << reverseFutilePruned;
    std::cout << ", Razor Pruned: " << razorPruned;
    std::cout << ", MultiCut Pruned: " << multiCutPruned;
    std::cout << ", Extensions: " << extensions;
    std::cout << ", IID Hits: " << iidHits << std::endl;
}

void SearchStats::clear()
{
    nodes = 0;
    qNodes = 0;
    cutoffs = 0;
    qCutoffs = 0;
    ttHits = 0;
    killersStored = 0;
    reSearches = 0;
    lmrReductions = 0;
    lmpPruned = 0;
    futileReductions = 0;
    futileReductionsQ = 0;
    deltaPruned = 0;
    seePruned = 0;
    nullReductions = 0;
    reverseFutilePruned = 0;
    razorPruned = 0;
    multiCutPruned = 0;
    extensions = 0;
    iidHits = 0;
}

/* IMPLEMENT AI METHODS */

// constructor/destructor
AI::AI()
{
    // set the pawn table size
    pawnTable_ = new PawnTable(PAWN_HASH_SIZE);
}

AI::~AI()
{
    // delete the pawn table
    delete pawnTable_;
}

// search
int AI::search(Board& board, TranspositionTable* transpositionTable_, int depth, int ply, int alpha, int beta, bool cut, bool& mainThreadStopped, std::chrono::steady_clock::time_point start)
{
    // check if main thread stopped
    if (mainThreadStopped)
    {
        return FAIL_SCORE;
    }

    // check for time
    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
    std::chrono::duration<double> elapsed = end - start;
    if (elapsed.count() >= MAX_TIME)
    {
        return FAIL_SCORE;
    }

    // check if this is a pv node
    bool pvNode = (beta - alpha) > 1;

    // increment nodes
    searchStats_.nodes++;

    // insufficient material
    if (board.insufficientMaterial(WHITE) && board.insufficientMaterial(BLACK))
    {
        return DRAW;
    }

    // check for threefold repetition
    if (ply > 0 && board.getHistory() > 1)
    {
        return DRAW;
    }

    // transposition table lookup
    int ttScore = transpositionTable_->getScore(board.getCurrentHash(), depth, ply, alpha, beta);
    Move ttMove = transpositionTable_->getMove(board.getCurrentHash());
    if (ttScore != FAIL_SCORE && ttMove.from != NONE)
    {
        if (ply == 0)
        {
            bestMoveCurrentIteration_ = ttMove;
            bestScoreCurrentIteration_ = ttScore;
        }

        searchStats_.ttHits++;
        return ttScore;
    }

    // check for max depth
    if (depth <= 0)
    {
        return quiesce(board, alpha, beta);
    }

    // generate moves
    board.moveGenerationSetup();
    Move moves[MAX_MOVES];
    int numMoves = 0;
    board.generateMoves(moves, numMoves);

    // check for mate/stalemate
    bool friendlyKingInCheck = board.getCheckers() != 0;
    if (numMoves == 0)
    {
        if (friendlyKingInCheck)
        {
            return -MATE + ply;
        }
        else
        {
            return DRAW;
        }
    }

    /******************* 
     *     EXTENSIONS 
     *******************/

    /******************* 
     *     PRUNING 
     *******************/

    // sort moves 
    scoreMoves(board, transpositionTable_, killerMoves_, moves, numMoves, ply);
    sortMoves(moves, numMoves);

    if (!friendlyKingInCheck && !pvNode)
    {
        // razoring
        if (razorOk(board, depth, alpha))
        {
            searchStats_.razorPruned++;
            depth -= 1;
        }

        // reverse futility pruning
        if (reverseFutileOk(board, depth, beta))
        {
            int score = lazyEvaluate(board);
            if (score - REVERSE_FUTILE_MARGINS[depth] >= beta)
            {
                searchStats_.reverseFutilePruned++;
                return score;
            }
        }

        // extended null move reductions
        if (nullOk(board, depth))
        {
            // make null move
            Square ep = board.makeNullMove();

            // get reduction
            int R = depth > 6 ? MAX_R : MIN_R;

            // search
            int score = -search(board, transpositionTable_, depth - R - 1, ply + 1, -beta, -beta + 1, !cut, mainThreadStopped, start);

            // undo null move
            board.unmakeNullMove(ep);

            // check for cutoff
            if (score >= beta)
            {
                searchStats_.nullReductions++;
                depth -= DR;
                if (depth <= 0)
                {
                    return evaluate(board, pawnTable_);
                }
            }
        }

        // multi-cut pruning
        if (cut && depth >= MULTI_CUT_R)
        {
            int c = 0;
            for (int i = 0; i < std::min(numMoves, MULTI_CUT_M); i++)
            {
                board.makeMove(moves[i]);
                int score = -search(board, transpositionTable_, depth-1-MULTI_CUT_R, ply + 1, -beta, -beta + 1, i != 0, mainThreadStopped, start);
                board.unmakeMove(moves[i]);
                if (score >= beta)
                {
                    if (++c == MULTI_CUT_C) 
                    {
                        searchStats_.multiCutPruned++;
                        return beta; // mc-prune
                    }
                }
            }
        }
    }

    /******************************
     * INTERNAL ITERATIVE DEEPENING 
     ******************************/
    if ((ttMove.from == NONE) && depth > MIN_IID_DEPTH)
    {
        // search with reduced depth
        search(board, transpositionTable_, depth - IID_DR, ply, alpha, beta, cut, mainThreadStopped, start);

        // check if the tt entry is now valid
        ttMove = transpositionTable_->getMove(board.getCurrentHash());
        if (ttMove.from != NONE)
        {
            searchStats_.iidHits++;
        }
    }

    // initialize transposition flag and best move
    Flag flag = UPPER_BOUND;
    Move bestMove = moves[0];

    /******************* 
     *     SEARCH 
     *******************/

    // loop through moves
    for (int i = 0; i < numMoves; i++)
    {
        // see if move causes check
        bool causesCheck = moveCausesCheck(board, moves[i]);
        bool pruningOk = !causesCheck && !friendlyKingInCheck && !pvNode;

        // late move pruning
        if (lmpOk(board, moves[i], i, depth) && pruningOk)
        {
            searchStats_.lmpPruned++;
            continue;
        }

        // futile pruning
        if (futile(board, moves[i], i, depth, alpha, beta) && pruningOk)
        {
            searchStats_.futileReductions++;
            continue;
        }

        // make move
        board.makeMove(moves[i]);

        // search
        int score;
        if (i == 0)
        {
            score = -search(board, transpositionTable_, depth - 1, ply + 1, -beta, -alpha, false, mainThreadStopped, start);
        }
        else
        {
            // LMR 
            int reduction = 0;
            if (lmrValid(board, moves[i], i, depth) && pruningOk) reduction = lmrReduction(i, depth);

            // get score
            score = -search(board, transpositionTable_, depth - 1 - reduction, ply + 1, -alpha - 1, -alpha, !cut, mainThreadStopped, start);
            searchStats_.lmrReductions++;

            // re-search if necessary
            if (score > alpha && reduction > 0)
            {
                score = -search(board, transpositionTable_, depth - 1, ply + 1, -beta, -alpha, !cut, mainThreadStopped, start);
                searchStats_.lmrReductions--;
                searchStats_.reSearches++;
            }
            else if (score > alpha && score < beta)
            {
                score = -search(board, transpositionTable_, depth - 1, ply + 1, -beta, -alpha, !cut, mainThreadStopped, start);
                searchStats_.reSearches++;
            }
        }

        // undo move
        board.unmakeMove(moves[i]);

        // check for cutoff
        if (score >= beta)
        {
            // store in transposition table
            transpositionTable_->store(board.getCurrentHash(), LOWER_BOUND, depth, ply, score, moves[i]);

            // store killer move/update history if quiet move
            if (moves[i].type <= QUEEN_CASTLE && abs(score) != abs(FAIL_SCORE))
            {
                Move firstKiller = killerMoves_[ply][0];

                // ensure that the killer move is not the same as the current move
                if (firstKiller.from != moves[i].from || firstKiller.to != moves[i].to || firstKiller.type != moves[i].type)
                {
                    killerMoves_[ply][1] = firstKiller;
                    killerMoves_[ply][0] = moves[i];
                    searchStats_.killersStored++;
                }
            }

            searchStats_.cutoffs++;
            return beta;
        }

        // update alpha
        if (score > alpha)
        {
            alpha = score;
            flag = EXACT;
            bestMove = moves[i];

            if (ply == 0)
            {
                bestMoveCurrentIteration_ = moves[i];
                bestScoreCurrentIteration_ = score;
            }
        }
    }

    // store in transposition table
    transpositionTable_->store(board.getCurrentHash(), flag, depth, ply, alpha, bestMove);

    // return alpha
    return alpha;
}

// quiescence search
int AI::quiesce(Board& board, int alpha, int beta)
{
    // increment qNodes
    searchStats_.qNodes++;

    // evaluate board
    int score = evaluate(board, pawnTable_);

    // check for cutoff
    if (score >= beta)
    {
        searchStats_.qCutoffs++;
        return beta;
    }

    // delta pruning
    int DELTA = PIECE_VALUES[QUEEN - 1];
    if (score + DELTA < alpha)
    {
        searchStats_.deltaPruned++;
        return alpha;
    }

    // update alpha
    if (score > alpha)
    {
        alpha = score;
    }

    // generate attacking moves
    board.moveGenerationSetup();
    Move moves[MAX_MOVES_ATTACK];
    int numMoves = 0;
    board.generateMoves(moves, numMoves, true);

    // sort moves
    scoreMoves(board, NULL, killerMoves_, moves, numMoves, -1);
    sortMoves(moves, numMoves);

    // loop through moves
    for (int i = 0; i < numMoves; i++)
    {
        if (moves[i].type < KNIGHT_PROMOTION && moves[i].type != EN_PASSANT) // dont prune promos or ep
        {
            int seeScore = see(board, moves[i].from, moves[i].to);

            // see pruning
            if (seeScore < 0)
            {
                searchStats_.seePruned++;
                continue;
            }

            // futility pruning
            if (score + seeScore + FUTILE_MARGIN_Q < alpha)
            {
                searchStats_.futileReductionsQ++;
                continue;
            }
        }

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

// threaded search method
Move threadedSearch(Board& board, TranspositionTable* transpositionTable_)
{
    // declare AIs and threads based on THREADS constant
    AI master = AI();
    AI* slaves[THREADS];
    Board* boards[THREADS];
    std::thread threads[THREADS];

    for (int i = 0; i < THREADS; i++)
    {
        boards[i] = new Board(board.getFen());
        slaves[i] = new AI();
    }

    // initialize variables
    Move bestMove = Move();
    int bestScore = 0;
    int depth = 1;
    int alpha = NEG_INF, beta = POS_INF;
    bool mainThreadStopped;

    // iterative deepening with time
    std::chrono::steady_clock::time_point start = std::chrono::steady_clock::now();
    while (depth <= MAX_DEPTH)
    {
        // reset main thread stopped
        mainThreadStopped = false;

        // start threads
        for (int i = 0; i < THREADS; i++)
        {
            threads[i] = std::thread(&AI::search, slaves[i], std::ref(*boards[i]), transpositionTable_, depth, 0, alpha, beta, false, std::ref(mainThreadStopped), start);
        }

        // main thread
        master.search(board, transpositionTable_, depth, 0, alpha, beta, false, mainThreadStopped, start);

        // stop threads
        mainThreadStopped = true;

        // join threads
        for (int i = 0; i < THREADS; i++)
        {
            threads[i].join();
        }

        // check for time
        std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
        std::chrono::duration<double> elapsed = end - start;
        if (elapsed.count() >= MAX_TIME)
        {
            break;
        }

        // update best move and score if the call was not broken prematurely
        bestMove = master.bestMoveCurrentIteration_;
        bestScore = master.bestScoreCurrentIteration_; 

        // print info
        std::cout << "depth: " << depth;
        std::cout << ", time: " << elapsed.count();
        std::cout << ", best move: " << indexToSquare(bestMove.from) << indexToSquare(bestMove.to);
        std::cout << ", score: " << bestScore / 100.0 << ", ";
        master.searchStats_.print();

        // check for mate
        if (bestScore >= MATE - MAX_DEPTH || bestScore <= -MATE + MAX_DEPTH)
        {
            break;
        }

        // increment depth
        depth++; 
    }

    // delete dynamic variables
    for (int i = 0; i < THREADS; i++)
    {
        delete boards[i];
        delete slaves[i];
    }

    // return best move
    return bestMove;
}


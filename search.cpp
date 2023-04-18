#include <iostream>
#include <chrono>
#include "search.h"
#include "evaluate.h"
#include "moveorder.h"
#include "searchboost.h"

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
    extensions = 0;
    iidHits = 0;
}

/* IMPLEMENT AI METHODS */

// constructor/destructor
AI::AI()
{
    // set the transposition table size
    transpositionTable_ = new TranspositionTable(TT_SIZE);

    // set the pawn table size
    pawnTable_ = new PawnTable(PAWN_HASH_SIZE);
}

AI::~AI()
{
    // delete the transposition table
    delete transpositionTable_;

    // delete the pawn table
    delete pawnTable_;
}

// search
int AI::search(Board& board, int depth, int ply, int alpha, int beta, auto start)
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
    if (ply > 0 && board.getHistory() > 1)
    {
        return DRAW;
    }

    // transposition table lookup
    int ttScore = transpositionTable_->getScore(board.getCurrentHash(), depth, ply, alpha, beta);
    if (ttScore != NEG_INF)
    {
        if (ply == 0)
        {
            bestMoveCurrentIteration_ = transpositionTable_->getMove(board.getCurrentHash());
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

    /******************* 
     *     EXTENSIONS 
     *******************/
    bool friendlyKingInCheck = board.getCheckers() != 0;

    /******************* 
     *     PRUNING 
     *******************/

    if (!friendlyKingInCheck)
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
            int score = -search(board, depth - R - 1, ply + 1, -beta, -beta + 1, start);

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
    }

    /******************************
     * INTERNAL ITERATIVE DEEPENING 
     ******************************/
    Move ttMove = transpositionTable_->getMove(board.getCurrentHash());
    if ((ttMove.from == ttMove.to) && depth > MIN_IID_DEPTH)
    {
        // search with reduced depth
        search(board, depth - IID_DR, ply, alpha, beta, start);

        // check if the tt entry is now valid
        ttMove = transpositionTable_->getMove(board.getCurrentHash());
        if (!(ttMove.from == ttMove.to))
        {
            searchStats_.iidHits++;
        }
    }

    // sort moves 
    scoreMoves(board, transpositionTable_, killerMoves_, moves, numMoves, ply);
    sortMoves(moves, numMoves);

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
        bool pruningOk = !causesCheck && !friendlyKingInCheck;

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
        if (!lmrValid(board, moves[i], i, depth) || !pruningOk)
        {
            score = -search(board, depth - 1, ply + 1, -beta, -alpha, start);
        }
        else
        {
            int reduction = lmrReduction(i, depth);
            score = -search(board, depth - 1 - reduction, ply + 1, -beta, -alpha, start);
            searchStats_.lmrReductions++;
            if (score > alpha)
            {
                score = -search(board, depth - 1, ply + 1, -beta, -alpha, start);
                searchStats_.reSearches++;
                searchStats_.lmrReductions--;
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
            if (moves[i].type <= QUEEN_CASTLE)
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
    board.generateMoves(moves, numMoves, true, true);

    // sort moves
    scoreMoves(board, transpositionTable_, killerMoves_, moves, numMoves, -1);
    sortMoves(moves, numMoves);

    // loop through moves
    for (int i = 0; i < numMoves; i++)
    {
        if (moves[i].type != EN_PASSANT)
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

// get best move through iterative deepening
Move AI::getBestMove(Board& board)
{
    // initialize variables
    Move bestMove = Move();
    int bestScore = 0;
    int depth = 1;
    int alpha = NEG_INF, beta = POS_INF;

    // clear transposition table and killer moves
    transpositionTable_->clear();
    for (int i = 0; i < MAX_DEPTH; i++)
    {
        killerMoves_[i][0] = Move();
        killerMoves_[i][1] = Move();
    }

    // iterative deepening with time
    auto start = std::chrono::high_resolution_clock::now();
    while (depth <= MAX_DEPTH)
    {
        // clear stats
        searchStats_.clear();

        // search
        search(board, depth, 0, alpha, beta, start);

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
        std::cout << ", score: " << bestScore / 100.0 << ", ";
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
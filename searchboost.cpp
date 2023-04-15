#include "searchboost.h"
#include "evaluate.h"
#include "bitboard.h"
#include <vector>
#include <algorithm>
#include <iostream>

// methods for boosting search
bool lmrValid(Board& board, Move move, int moveIndex, int depth)
{
    // if moveIndex is greater than LMR_LIMIT, return false
    if (moveIndex > LMR_LIMIT)
    {
        return false;
    }

    // if depth is less than 3, return false
    if (depth < 3)
    {
        return false;
    }

    // if move is not quiet, return false
    if (move.type != QUIET)
    {
        return false;
    }

    // if we are in check, return false
    if (board.inCheckFull())
    {
        return false;
    }

    return true;
}

int lmrReduction(int moveIndex, int depth)
{
    if (moveIndex < 6) return 1;

    return depth / 3;
}

bool futile(Board& board, Move move, int moveIndex, int depth, int alpha, int beta)
{
    // if depth is greater than FUTILE_MAX_DEPTH, return false
    if (depth > FUTILE_MAX_DEPTH)
    {
        return false;
    }

    // if first move, return false
    if (moveIndex == 0)
    {
        return false;
    }

    // if move is not quiet, return false
    if (move.type != QUIET && move.type != KING_CASTLE && move.type != QUEEN_CASTLE)
    {
        return false;
    }

    // if we are in check, return false
    if (board.inCheckFull())
    {
        return false;
    }

    // if alpha or beta is near checkmate, return false
    if (abs(alpha) > MATE - 100 || abs(beta) > MATE - 100)
    {
        return false;
    }

    // margin
    int margin = FUTILE_MARGINS[depth];
    
    // positional gain
    Color color = board.getNextMove();
    Piece piece = extractPiece(board.getSquareToPiece(move.from));
    int openingFrom = TABLES[color][piece-1][0][move.from], openingTo = TABLES[color][piece-1][0][move.to];
    int endgameFrom = TABLES[color][piece-1][1][move.from], endgameTo = TABLES[color][piece-1][1][move.to];
    int phase = board.getPhase();
    int fromScore = (openingFrom * (256 - phase) + endgameFrom * phase) / 256;
    int toScore = (openingTo * (256 - phase) + endgameTo * phase) / 256;
    int gain = toScore - fromScore;

    // if move could raise alpha, return false. otherwise, return true
    if (lazyEvaluate(board) + gain + margin <= alpha)
    {
        return true;
    }

    return false;
}

int see(Board& board, Square initialFrom, Square initialTo) // bugged
{
    // get a bitboard of all the shallow (not considering x-ray) attackers of the "to" square
    UInt64 fullAttackers = board.getAttackersForSquare(initialTo);
    UInt64 shallowAttackers = fullAttackers;

    // pseudo-occupancy bitboard
    UInt64 occupancy = board.getFullOccupied();

    // loop through the attackers and sort them into a list by their value
    std::vector<Square> attackers;
    std::vector<Square> defenders;
    while (shallowAttackers)
    {
        // get the least significant bit
        Square attacker = static_cast<Square>(lsb(shallowAttackers));

        // remove the least significant bit
        shallowAttackers ^= (1ULL << attacker);

        // if attacker is piece on initialFrom, skip it (we will add it later)
        if (attacker == initialFrom)
        {
            continue;
        }

        // add piece to appropriate list
        Color color = extractColor(board.getSquareToPiece(attacker));
        if (color == board.getNextMove())
        {
            attackers.push_back(attacker);
        }
        else
        {
            defenders.push_back(attacker);
        }
    }

    // sort the attackers and defenders by value (most valuable first)
    std::sort(attackers.begin(), attackers.end(), [&board](Square a, Square b) {
        return PIECE_VALUES[extractPiece(board.getSquareToPiece(a)) - 1] > PIECE_VALUES[extractPiece(board.getSquareToPiece(b)) - 1];
    });
    std::sort(defenders.begin(), defenders.end(), [&board](Square a, Square b) {
        return PIECE_VALUES[extractPiece(board.getSquareToPiece(a)) - 1] > PIECE_VALUES[extractPiece(board.getSquareToPiece(b)) - 1];
    });

    // add the piece on initialFrom to the attackers list
    attackers.push_back(initialFrom);

    // loop
    int score = 0;
    int mode = 1; // 0 = enemy defending, 1 = ally attacking
    Piece pieceOnToSquare = extractPiece(board.getSquareToPiece(initialTo));
    while (true)
    {
        // if we're in ally attacking mode and there are no attackers, break
        if (mode == 1 && attackers.size() == 0)
        {
            break;
        }

        // if we're in enemy defending mode and there are no defenders, break
        if (mode == 0 && defenders.size() == 0)
        {
            break;
        }

        // get the next piece and pop it off the list end
        Square nextPieceSquare;
        if (mode == 1)
        {
            nextPieceSquare = attackers.back();
            attackers.pop_back();
        }
        else
        {
            nextPieceSquare = defenders.back();
            defenders.pop_back();
        }

        // get the piece on the square
        Piece nextPiece = extractPiece(board.getSquareToPiece(nextPieceSquare));

        // simulate the capture
        Piece capturedPiece = pieceOnToSquare;
        pieceOnToSquare = nextPiece;
        occupancy ^= (1ULL << nextPieceSquare);

        // get the new score
        if (mode == 1)
        {
            score += PIECE_VALUES[capturedPiece - 1];
        }
        else
        {
            score -= PIECE_VALUES[capturedPiece - 1];
        }

        // if it is the ally's turn and the score is negative, return score because defender can stand pat
        if (mode == 1 && score < 0)
        {
            return score;
        }

        // if it is the enemy's turn and the score is positive, return score because attacker can stand pat
        if (mode == 0 && score >= 0)
        {
            return score;
        }

        // add any discovered attackers to the list (if the piece is a queen, rook, or bishop)
        if (nextPiece != KNIGHT) // knights cannot be pinned
        {
            int row = nextPieceSquare / 8, col = nextPieceSquare % 8;
            if ((FILE_MASKS[col] & (1ULL << initialTo)) || (RANK_MASKS[row] & (1ULL << initialTo))) // if the piece is on the same file or rank as the initial capture
            {
                shallowAttackers = lookupRookAttack(initialTo, occupancy ^ (1ULL << initialTo)) & (board.getPiece(static_cast<Color>(1-board.getNextMove()), ROOK) | board.getPiece(static_cast<Color>(1-board.getNextMove()), QUEEN) | board.getPiece(board.getNextMove(), ROOK) | board.getPiece(board.getNextMove(), QUEEN)) & ~fullAttackers;
            }
            else
            {
                shallowAttackers = lookupBishopAttack(initialTo, occupancy ^ (1ULL << initialTo)) & (board.getPiece(static_cast<Color>(1-board.getNextMove()), BISHOP) | board.getPiece(static_cast<Color>(1-board.getNextMove()), QUEEN) | board.getPiece(board.getNextMove(), BISHOP) | board.getPiece(board.getNextMove(), QUEEN)) & ~fullAttackers;
            }

            // add the discovered attackers to the list
            fullAttackers |= shallowAttackers;
            while (shallowAttackers)
            {
                // get the least significant bit
                Square attacker = static_cast<Square>(lsb(shallowAttackers));

                // remove the least significant bit
                shallowAttackers ^= (1ULL << attacker);

                // add piece to appropriate list
                Color color = extractColor(board.getSquareToPiece(attacker));
                if (color == board.getNextMove())
                {
                    attackers.push_back(attacker);
                }
                else
                {
                    defenders.push_back(attacker);
                }
            }

            // sort the attackers and defenders by value (most valuable first)
            std::sort(attackers.begin(), attackers.end(), [&board](Square a, Square b) {
                return PIECE_VALUES[extractPiece(board.getSquareToPiece(a)) - 1] > PIECE_VALUES[extractPiece(board.getSquareToPiece(b)) - 1];
            });
            std::sort(defenders.begin(), defenders.end(), [&board](Square a, Square b) {
                return PIECE_VALUES[extractPiece(board.getSquareToPiece(a)) - 1] > PIECE_VALUES[extractPiece(board.getSquareToPiece(b)) - 1];
            });
        }

        // switch modes
        mode = 1 - mode;
    }

    // return the score
    return score;
}
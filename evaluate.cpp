#include "evaluate.h"
#include "bitboard.h"
#include <iostream>

// lazy evaluation
int lazyEvaluate(Board& board)
{
    int phase = board.getPhase();

    // material
    int openingScore = board.getMaterial(WHITE) - board.getMaterial(BLACK);
    int endgameScore = openingScore;

    // bishop pair
    if (hasBishopPair(board, WHITE))
    {
        openingScore += BISHOP_PAIR;
        endgameScore += BISHOP_PAIR;
    }
    if (hasBishopPair(board, BLACK))
    {
        openingScore -= BISHOP_PAIR;
        endgameScore -= BISHOP_PAIR;
    }

    // tempo bonus
    openingScore += TEMPO_BONUS * (board.getNextMove() == WHITE ? 1 : -1);

    // piece square tables
    openingScore += board.getStaticEvalOpening();
    endgameScore += board.getStaticEvalEndgame();

    // interpolate between opening and endgame
    int score = (openingScore * (256 - phase) + endgameScore * phase) / 256;

    // return the score based on the side to move
    return score * (board.getNextMove() == WHITE ? 1 : -1);
}

// eval function
int evaluate(Board& board)
{
    int phase = board.getPhase();

    // material
    int openingScore = board.getMaterial(WHITE) - board.getMaterial(BLACK);
    int endgameScore = openingScore;

    // bishop pair
    if (hasBishopPair(board, WHITE))
    {
        openingScore += BISHOP_PAIR;
        endgameScore += BISHOP_PAIR;
    }
    if (hasBishopPair(board, BLACK))
    {
        openingScore -= BISHOP_PAIR;
        endgameScore -= BISHOP_PAIR;
    }

    // knight outpost
    openingScore += knightOutpostScore(board, WHITE) - knightOutpostScore(board, BLACK);

    // adjust knight and rook values based on pawn count
    int whitePawnCount = board.getPieceCount(WHITE, PAWN);
    int blackPawnCount = board.getPieceCount(BLACK, PAWN);
    int whiteKnightCount = board.getPieceCount(WHITE, KNIGHT);
    int blackKnightCount = board.getPieceCount(BLACK, KNIGHT);
    int whiteRookCount = board.getPieceCount(WHITE, ROOK);
    int blackRookCount = board.getPieceCount(BLACK, ROOK);
    openingScore -= (8 - whitePawnCount) * whiteKnightCount * KNIGHT_DECREASE_WITH_PAWN_LOSS;
    openingScore += (8 - blackPawnCount) * blackKnightCount * KNIGHT_DECREASE_WITH_PAWN_LOSS;
    openingScore += (8 - whitePawnCount) * whiteRookCount * ROOK_INCREASE_WITH_PAWN_LOSS;
    openingScore -= (8 - blackPawnCount) * blackRookCount * ROOK_INCREASE_WITH_PAWN_LOSS;

    // tempo bonus
    openingScore += TEMPO_BONUS * (board.getNextMove() == WHITE ? 1 : -1);

    // piece square tables
    openingScore += board.getStaticEvalOpening();
    endgameScore += board.getStaticEvalEndgame();

    // interpolate between opening and endgame
    int score = (openingScore * (256 - phase) + endgameScore * phase) / 256;

    // return the score based on the side to move
    return score * (board.getNextMove() == WHITE ? 1 : -1);
}

// bishop 
bool hasBishopPair(Board& board, Color color)
{
    return board.getPieceCount(color, BISHOP) >= 2;
}

// knight
bool isKnightOutpost(Board& board, Color color, Square square)
{
    // get pawn attacks of opposte color on this square and check if it is defended by same color pawn
    UInt64 enemyPawnAttack = PAWN_ATTACKS[1 - color][square];

    // check if there is a pawn on the square and knight is on enemy side
    return (enemyPawnAttack & board.getPiece(color, PAWN)) && (SIDES[1-color] & (1ULL << square));
}

bool isKnightOnHole(Board& board, Color color, Square square)
{
    // get pawn attacks of current color on this square and check if it is attacked by opposite color pawn
    UInt64 friendlyPawnAttack = PAWN_ATTACKS[color][square];

    // check if there is a pawn on the square
    return !(friendlyPawnAttack & board.getPiece(static_cast<Color>(1 - color), PAWN));
}

int knightOutpostScore(Board& board, Color color)
{
    int score = 0;

    // loop through all knights
    UInt64 knights = board.getPiece(color, KNIGHT);
    while (knights)
    {
        Square square = static_cast<Square>(lsb(knights));
        if (isKnightOutpost(board, color, square))
        {
            score += KNIGHT_OUTPOST;
            if (isKnightOnHole(board, color, square))
            {
                score += KNIGHT_OUTPOST_ON_HOLE_BONUS;
            }
        }
        knights &= knights - 1;
    }

    return score;
}
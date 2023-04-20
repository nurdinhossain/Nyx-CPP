#include "moveorder.h"

// score moves based on MVV/LVA
void scoreMoves(Board& board, TranspositionTable* tt, Move killerMoves[][2], Move moves[], int numMoves, int ply) 
{
    // check for tt move
    Move ttMove = tt->getMove(board.getCurrentHash());

    for (int i = 0; i < numMoves; i++) 
    {
        Move move = moves[i];

        // check for tt move
        if (move.from == ttMove.from && move.to == ttMove.to && move.type == ttMove.type)
        {
            moves[i].score = TT_MOVE;
            continue;
        }

        // check for capture
        if (move.pieceTaken != EMPTY)
        {
            // get piece moved
            int piece = board.getSquareToPiece(move.from);
            Piece pieceMoved = extractPiece(piece);

            // score move 
            moves[i].score = MVV_LVA[move.pieceTaken-1][pieceMoved-1] + CAPTURE_OFFSET;

            // check for promotion capture
            if (move.type >= KNIGHT_PROMOTION_CAPTURE && move.type <= QUEEN_PROMOTION_CAPTURE)
            {
                // score move based on promotion piece
                moves[i].score += PIECE_VALUES[move.type - 9] + PROMO_OFFSET;
            }

            continue;
        }

        // check for promotion
        if (move.type >= KNIGHT_PROMOTION && move.type <= QUEEN_PROMOTION)
        {
            // score move based on promotion piece
            moves[i].score = PIECE_VALUES[move.type - 5] + PROMO_OFFSET;
            continue;
        }

        // check for killer move
        if (ply >= 0)
            {
            if (move.from == killerMoves[ply][0].from && move.to == killerMoves[ply][0].to && move.type == killerMoves[ply][0].type)
            {
                moves[i].score = CAPTURE_OFFSET;
                continue;
            }
            else if (move.from == killerMoves[ply][1].from && move.to == killerMoves[ply][1].to && move.type == killerMoves[ply][1].type)
            {
                moves[i].score = CAPTURE_OFFSET - KILLER_VALUE;
                continue;
            }
        }

        // get positional gain from move
        Color color = board.getNextMove();
        Piece piece = extractPiece(board.getSquareToPiece(move.from));
        int openingFrom = TABLES[piece-1][0][getTableIndex(move.from, color)], openingTo = TABLES[piece-1][0][getTableIndex(move.to, color)];
        int endgameFrom = TABLES[piece-1][1][getTableIndex(move.from, color)], endgameTo = TABLES[piece-1][1][getTableIndex(move.to, color)];
        int phase = board.getPhase();
        int fromScore = (openingFrom * (256 - phase) + endgameFrom * phase) / 256;
        int toScore = (openingTo * (256 - phase) + endgameTo * phase) / 256;
        moves[i].score = toScore - fromScore;  
    }
}

// sort moves from highest score to lowest score using quicksort
void sortMoves(Move moves[], int numMoves) 
{
    // check for no moves
    if (numMoves == 0)
    {
        return;
    }

    // quicksort
    int i = 0;
    int j = numMoves - 1;
    Move pivot = moves[numMoves / 2];
    while (i <= j) 
    {
        while (moves[i].score > pivot.score)
        {
            i++;
        }
        while (moves[j].score < pivot.score)
        {
            j--;
        }
        if (i <= j) 
        {
            Move temp = moves[i];
            moves[i] = moves[j];
            moves[j] = temp;
            i++;
            j--;
        }
    }
    if (j > 0)
    {
        sortMoves(moves, j + 1);
    }
    if (i < numMoves)
    {
        sortMoves(moves + i, numMoves - i);
    }
}
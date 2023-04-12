#include "moveorder.h"

// score moves based on MVV/LVA
void scoreMoves(Board board, Move moves[], int numMoves) 
{
    for (int i = 0; i < numMoves; i++) 
    {
        Move move = moves[i];

        // check for capture
        if (move.pieceTaken != EMPTY)
        {
            // get piece moved
            int piece = board.getSquareToPiece(move.from);
            Piece pieceMoved = extractPiece(piece);

            // score move
            moves[i].score += MVV_LVA[move.pieceTaken-1][pieceMoved-1];
        }

        // check for promotion
        if (move.type >= KNIGHT_PROMOTION && move.type <= QUEEN_PROMOTION)
        {
            // score move based on promotion piece
            moves[i].score += PIECE_VALUES[move.type - 5];
        }
        else if (move.type >= KNIGHT_PROMOTION_CAPTURE && move.type <= QUEEN_PROMOTION_CAPTURE)
        {
            // score move based on promotion piece
            moves[i].score += PIECE_VALUES[move.type - 9];
        }

        // get positional gain from move
        Color color = board.getNextMove();
        Piece piece = extractPiece(board.getSquareToPiece(move.from));
        int openingGain = TABLES[color][piece-1][0][move.to] - TABLES[color][piece-1][0][move.from];
        int endgameGain = TABLES[color][piece-1][1][move.to] - TABLES[color][piece-1][1][move.from];
        moves[i].score += openingGain + endgameGain;
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
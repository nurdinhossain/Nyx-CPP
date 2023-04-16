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

    // pawn score
    int whiteOpeningPawnScore = 0, whiteEndgamePawnScore = 0;
    int blackOpeningPawnScore = 0, blackEndgamePawnScore = 0;
    pawnScore(board, WHITE, whiteOpeningPawnScore, whiteEndgamePawnScore);
    pawnScore(board, BLACK, blackOpeningPawnScore, blackEndgamePawnScore);
    openingScore += whiteOpeningPawnScore - blackOpeningPawnScore;
    endgameScore += whiteEndgamePawnScore - blackEndgamePawnScore;

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

    // rook score
    int whiteOpeningRookScore = 0, whiteEndgameRookScore = 0;
    int blackOpeningRookScore = 0, blackEndgameRookScore = 0;
    rookScore(board, WHITE, whiteOpeningRookScore, whiteEndgameRookScore);
    rookScore(board, BLACK, blackOpeningRookScore, blackEndgameRookScore);
    openingScore += whiteOpeningRookScore - blackOpeningRookScore;
    endgameScore += whiteEndgameRookScore - blackEndgameRookScore;

    // king score
    int whiteOpeningKingScore = 0, whiteEndgameKingScore = 0;
    int blackOpeningKingScore = 0, blackEndgameKingScore = 0;
    kingScore(board, WHITE, whiteOpeningKingScore, whiteEndgameKingScore);
    kingScore(board, BLACK, blackOpeningKingScore, blackEndgameKingScore);
    openingScore += whiteOpeningKingScore - blackOpeningKingScore;
    endgameScore += whiteEndgameKingScore - blackEndgameKingScore;

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

// rook
bool kingBlockRook(Board& board, Color color, Square rookSquare)
{
    // get king square
    Square kingSquare = static_cast<Square>(lsb(board.getPiece(color, KING)));

    // check for patterns
    if (color == WHITE)
    {
        if (rookSquare == h1)
        {
            if (kingSquare == g1 || kingSquare == f1)
            {
                return true;
            }
        }
        else if (rookSquare == a1)
        {
            if (kingSquare == b1 || kingSquare == c1 || kingSquare == d1)
            {
                return true;
            }
        }
    }
    else
    {
        if (rookSquare == h8)
        {
            if (kingSquare == g8 || kingSquare == f8)
            {
                return true;
            }
        }
        else if (rookSquare == a8)
        {
            if (kingSquare == b8 || kingSquare == c8 || kingSquare == d8)
            {
                return true;
            }
        }
    }

    return false;
}

void rookScore(Board& board, Color color, int& openingScore, int& endgameScore)
{
    // loop through all rooks
    UInt64 rooks = board.getPiece(color, ROOK);
    while (rooks)
    {
        Square square = static_cast<Square>(lsb(rooks));

        // check for 7th rank
        int rank = square / 8, file = square % 8;
        if (color == WHITE && rank == 6)
        {
            openingScore += ROOK_ON_SEVENTH;
            endgameScore += ROOK_ON_SEVENTH;
        }
        else if (color == BLACK && rank == 1)
        {
            openingScore += ROOK_ON_SEVENTH;
            endgameScore += ROOK_ON_SEVENTH;
        }

        // check if rook is trapped by king
        if (kingBlockRook(board, color, square))
        {
            openingScore -= KING_BLOCK_ROOK_PENALTY;
        }

        // check for open/half open file
        UInt64 rookAttack = lookupRookAttack(square, board.getFullOccupied() ^ (1ULL << square));
        UInt64 fileMask = FILE_MASKS[file];
        int fileCount = popCount(rookAttack & fileMask & ~(board.getFullOccupied()));
        if (fileCount >= 6)
        {
            openingScore += ROOK_OPEN_FILE;

            if (file == 0 || file == 7)
            {
                openingScore += ROOK_CONTROL_AH_FILE;
            }
        }
        else if (fileCount >= 4)
        {
            openingScore += ROOK_HALF_OPEN_FILE;
        }

        // check for connected rooks
        if (rookAttack & board.getPiece(color, ROOK))
        {
            openingScore += ROOKS_CONNECTED / 2;
            endgameScore += ROOKS_CONNECTED / 2;
        }

        rooks &= rooks - 1;
    }
}

// pawn
bool isPassedPawn(Board& board, Color color, Square square)
{
    int rank = square / 8, file = square % 8;
    UInt64 neighboringFiles = NEIGHBORING_FILES[file];
    UInt64 frontSpan = color == WHITE ? SQUARES_ABOVE_WHITE_PAWNS[rank] : SQUARES_BELOW_BLACK_PAWNS[rank];

    return !(frontSpan & neighboringFiles & board.getPiece(static_cast<Color>(1 - color), PAWN));
}

bool pawnIsObstructed(Board& board, Color color, Square square)
{
    int rank = square / 8, file = square % 8;
    UInt64 currentFile = FILE_MASKS[file];
    UInt64 frontSpan = color == WHITE ? SQUARES_ABOVE_WHITE_PAWNS[rank] : SQUARES_BELOW_BLACK_PAWNS[rank];

    if (SIDES[color] & (1ULL << square))
    {
        return false;
    }

    return frontSpan & currentFile & board.getFullOccupied();
}

void pawnScore(Board& board, Color color, int& openingScore, int& endgameScore)
{
    // loop through all pawns
    UInt64 pawns = board.getPiece(color, PAWN);
    while (pawns)
    {
        Square square = static_cast<Square>(lsb(pawns));
        int file = square % 8;

        // unprotected pawn
        bool isProtected = PAWN_ATTACKS[1-color][square] & board.getPiece(color, PAWN);
        if (!isProtected)
        {
            openingScore -= UNPROTECTED_PAWN_PENALTY;
        }

        // check for passed pawns
        if (isPassedPawn(board, color, square))
        {
            openingScore += PASSED_PAWN;
            endgameScore += PASSED_PAWN;

            // protected passed pawn
            if (isProtected)
            {
                openingScore += PROTECTED_PASSED_PAWN;
                endgameScore += PROTECTED_PASSED_PAWN;
            }

            // outside passed pawn
            if (file == 0 || file == 7)
            {
                endgameScore += OUTSIDE_PASSED_PAWN;
            }

            // unobstructed passed pawn
            if (!pawnIsObstructed(board, color, square))
            {
                endgameScore += UNOBSTRUCTED_PASSED_PAWN;
            }

            // isolated pawn
            UInt64 adjacentFiles = NEIGHBORING_FILES[file] ^ FILE_MASKS[file];
            if (!(adjacentFiles & board.getPiece(color, PAWN)))
            {
                openingScore -= ISOLATED_PAWN_PENALTY;
            }
        }

        pawns &= pawns - 1;
    }
}

// king
int kingPawnShieldScore(Board& board, Color color, Square square)
{
    UInt64 kingAttack = KING_ATTACKS[square];
    UInt64 kingFrontSpan = color == WHITE ? SQUARES_ABOVE_WHITE_PAWNS[square / 8] : SQUARES_BELOW_BLACK_PAWNS[square / 8];

    return popCount(kingAttack & kingFrontSpan & board.getPiece(color, PAWN));
}

void kingScore(Board& board, Color color, int& openingScore, int& endgameScore)
{
    // get king square   
    Square kingIndex = static_cast<Square>(lsb(board.getPiece(color, KING)));

    // check for king pawn shield
    openingScore += kingPawnShieldScore(board, color, kingIndex) * PAWN_SHIELD;

    // check for king on open file
    int file = kingIndex % 8;
    UInt64 rookAttackFromKingSquare = lookupRookAttack(kingIndex, board.getFullOccupied() ^ (1ULL << kingIndex));
    UInt64 fileMask = FILE_MASKS[file];
    int fileCount = popCount(rookAttackFromKingSquare & fileMask & ~(board.getFullOccupied()));
    if (fileCount >= 5)
    {
        openingScore -= KING_OPEN_FILE_PENALTY;
    }

    // check for king next to open file
    if (file == 0)
    {
        Square adjacentSquare = static_cast<Square>(kingIndex + 1);
        UInt64 rookAttackFromAdjacentSquare = lookupRookAttack(adjacentSquare, board.getFullOccupied() & ~(1ULL << adjacentSquare));
        int adjacentFileCount = popCount(rookAttackFromAdjacentSquare & FILE_MASKS[adjacentSquare % 8] & ~(board.getFullOccupied()));
        if (adjacentFileCount >= 5)
        {
            openingScore -= KING_NEXT_TO_OPEN_FILE_PENALTY;
        }
    }
    else if (file == 7)
    {
        Square adjacentSquare = static_cast<Square>(kingIndex - 1);
        UInt64 rookAttackFromAdjacentSquare = lookupRookAttack(adjacentSquare, board.getFullOccupied() & ~(1ULL << adjacentSquare));
        int adjacentFileCount = popCount(rookAttackFromAdjacentSquare & FILE_MASKS[adjacentSquare % 8] & ~(board.getFullOccupied()));
        if (adjacentFileCount >= 5)
        {
            openingScore -= KING_NEXT_TO_OPEN_FILE_PENALTY;
        }
    }
    else {
        Square adjacentSquares[] = { static_cast<Square>(kingIndex + 1), static_cast<Square>(kingIndex - 1) };
        for (Square adjacentSquare : adjacentSquares)
        {
            UInt64 rookAttackFromAdjacentSquare = lookupRookAttack(adjacentSquare, board.getFullOccupied() & ~(1ULL << adjacentSquare));
            int adjacentFileCount = popCount(rookAttackFromAdjacentSquare & FILE_MASKS[adjacentSquare % 8] & ~(board.getFullOccupied()));
            if (adjacentFileCount >= 5)
            {
                openingScore -= KING_NEXT_TO_OPEN_FILE_PENALTY;
            }
        }
    }
}
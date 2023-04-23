#include "evaluate.h"
#include "bitboard.h"
#include "evalparams.h"
#include <iostream>

// lazy evaluation
int lazyEvaluate(Board& board)
{
    int phase = board.getPhase();

    // material
    int openingScore = board.getMaterial(WHITE) - board.getMaterial(BLACK);

    // ensure endgame score equals opening score for material
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

    // king score
    int whiteOpeningKingScore = 0, whiteEndgameKingScore = 0;
    int blackOpeningKingScore = 0, blackEndgameKingScore = 0;
    kingScore(board, WHITE, whiteOpeningKingScore, whiteEndgameKingScore);
    kingScore(board, BLACK, blackOpeningKingScore, blackEndgameKingScore);
    openingScore += whiteOpeningKingScore - blackOpeningKingScore;
    endgameScore += whiteEndgameKingScore - blackEndgameKingScore;

    // piece square tables
    openingScore += board.getStaticEvalOpening();
    endgameScore += board.getStaticEvalEndgame();

    // interpolate between opening and endgame
    int score = (openingScore * (256 - phase) + endgameScore * phase) / 256;

    // return the score based on the side to move
    return score * (board.getNextMove() == WHITE ? 1 : -1);
}

// eval function
int evaluate(Board& board, PawnTable* pawnTable)
{
    int phase = board.getPhase();

    // material
    int openingScore = board.getMaterial(WHITE) - board.getMaterial(BLACK);

    // ensure endgame score equals opening score for material
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

    // pawn score
    
    // check pawn table for pawn score
    PawnEntry* entry = pawnTable->probe(board.getPawnHash());
    if (entry->key == board.getPawnHash())
    {
        openingScore += entry->openingScore;
        endgameScore += entry->endgameScore;
    }
    else
    {
        int whiteOpeningPawnScore = 0, whiteEndgamePawnScore = 0;
        int blackOpeningPawnScore = 0, blackEndgamePawnScore = 0;
        pawnScore(board, WHITE, whiteOpeningPawnScore, whiteEndgamePawnScore);
        pawnScore(board, BLACK, blackOpeningPawnScore, blackEndgamePawnScore);

        // store pawn score in pawn table
        int openingScoreContrib = whiteOpeningPawnScore - blackOpeningPawnScore;
        int endgameScoreContrib = whiteEndgamePawnScore - blackEndgamePawnScore;
        pawnTable->store(board.getPawnHash(), openingScoreContrib, endgameScoreContrib);

        openingScore += openingScoreContrib;
        endgameScore += endgameScoreContrib;
    }

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

void bishopMobility(Board& board, Color color, int& openingScore, int& endgameScore)
{
    UInt64 bishops = board.getPiece(color, BISHOP);
    while (bishops)
    {
        // get the least significant bit
        Square square = static_cast<Square>(lsb(bishops));

        // get the mobility of the bishop
        UInt64 bishopAttack = lookupBishopAttack(square, board.getFullOccupied() ^ (1ULL << square));
        int mobility = popCount(bishopAttack & SIDES[1-color] & ~board.getFullOccupied()); // moves on enemy side

        // add to score
        openingScore += BISHOP_MOBILITY * mobility;

        // remove the least significant bit
        bishops &= bishops - 1;
    }
}

// knight
bool isHole(Board& board, Color color, Square square)
{
    // get frontspan of knight 
    int file = square % 8, rank = square / 8;
    UInt64 frontSpan = color == WHITE ? SQUARES_ABOVE_WHITE_PAWNS[rank] : SQUARES_BELOW_BLACK_PAWNS[rank];
    frontSpan &= (NEIGHBORING_FILES[file] ^ FILE_MASKS[file]);

    // if knight can't ever be attacked by a pawn, return true
    UInt64 enemyPawns = board.getPiece(static_cast<Color>(1-color), PAWN);
    if (!(frontSpan & enemyPawns))
        return true;

    return false;
}

bool isKnightOutpost(Board& board, Color color, Square square)
{
    // get pawn attacks of opposte color on this square and check if it is defended by same color pawn
    UInt64 enemyPawnAttack = PAWN_ATTACKS[1 - color][square];

    // check if there is a pawn on the square and knight is on enemy side
    return (enemyPawnAttack & board.getPiece(color, PAWN)) && (SIDES[1-color] & (1ULL << square));
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

            if (isHole(board, color, square))
                score += KNIGHT_OUTPOST_ON_HOLE;
        }
        knights &= knights - 1;
    }

    return score;
}

// rook
bool openFile(Board& board, Color color, int file)
{
    Color enemy = static_cast<Color>(1 - color);
    UInt64 allyPawns = board.getPiece(color, PAWN), enemyPawns = board.getPiece(enemy, PAWN);

    // return true if there are no pawns on the file
    return !((allyPawns | enemyPawns) & FILE_MASKS[file]);
}

bool halfOpenFile(Board& board, Color color, int file)
{
    Color enemy = static_cast<Color>(1 - color);
    UInt64 allyPawns = board.getPiece(color, PAWN), enemyPawns = board.getPiece(enemy, PAWN);

    // return true if we don't have a pawn but enemy does
    return !(allyPawns & FILE_MASKS[file]) && (enemyPawns & FILE_MASKS[file]);
}

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

        // check if rook is on open file or half open file
        int file = square % 8;
        if (openFile(board, color, file))
        {
            openingScore += ROOK_OPEN_FILE;
        }

        // check if rook is blocked by king
        if (kingBlockRook(board, color, square))
        {
            openingScore -= KING_BLOCK_ROOK_PENALTY;
        }

        rooks &= rooks - 1;
    }
}

// pawn
bool isPassed(Board& board, Color color, Square square)
{
    int rank = square / 8, file = square % 8;
    UInt64 neighboringFiles = NEIGHBORING_FILES[file];
    UInt64 frontSpan = color == WHITE ? SQUARES_ABOVE_WHITE_PAWNS[rank] : SQUARES_BELOW_BLACK_PAWNS[rank];

    // if pawn is obstructed by ally pawns, it is not passed
    if (frontSpan & FILE_MASKS[file] & board.getPiece(color, PAWN))
    {
        return false;
    }

    return !(frontSpan & neighboringFiles & board.getPiece(static_cast<Color>(1 - color), PAWN));
}

bool isProtected(Board& board, Color color, Square square)
{
    return PAWN_ATTACKS[1 - color][square] & board.getPiece(color, PAWN);
} 

bool isOutside(Board& board, Color color, Square square)
{
    int file = square % 8;
    return file == 0 || file == 7;
}

bool isIsolated(Board& board, Color color, Square square)
{
    int file = square % 8;
    UInt64 neighboringFiles = NEIGHBORING_FILES[file] ^ FILE_MASKS[file];
    return !(neighboringFiles & board.getPiece(color, PAWN));
}

bool isBackward(Board& board, Color color, Square square)
{
    // get the backspan of the pawn
    int rank = square / 8;
    UInt64 backSpan = color == WHITE ? SQUARES_BELOW_BLACK_PAWNS[rank] : SQUARES_ABOVE_WHITE_PAWNS[rank];
    UInt64 neighboringFiles = NEIGHBORING_FILES[square % 8] ^ FILE_MASKS[square % 8];
    if (backSpan & neighboringFiles & board.getPiece(color, PAWN))
    {
        return false;
    }

    // get the stop square of the pawn
    Square stopSquare = color == WHITE ? static_cast<Square>(square + 8) : static_cast<Square>(square - 8);

    // if stop square is protected by ally pawn, then it is not backward
    if (isProtected(board, color, stopSquare))
    {
        return false;
    }

    // if stop square is not attacked by enemy pawn, then it is not backward
    if (!isProtected(board, static_cast<Color>(1 - color), stopSquare))
    {
        return false;
    }

    return true;
}

bool isDoubled(Board& board, Color color, Square square)
{
    int file = square % 8;
    return popCount(FILE_MASKS[file] & board.getPiece(color, PAWN)) > 1;
}

void pawnScore(Board& board, Color color, int& openingScore, int& endgameScore)
{
    // loop through all pawns
    UInt64 pawns = board.getPiece(color, PAWN);
    while (pawns)
    {
        Square square = static_cast<Square>(lsb(pawns));

        // check if pawn is protected
        bool pawnIsProtected = isProtected(board, color, square);

        // check if pawn is passed
        if (isPassed(board, color, square))
        {
            openingScore += PASSED_PAWN;
            endgameScore += PASSED_PAWN;

            // check if pawn is outside
            if (isOutside(board, color, square))
            {
                endgameScore += OUTSIDE_PASSED_PAWN;
            }
        }

        // check if pawn is isolated
        if (isIsolated(board, color, square))
        {
            openingScore -= ISOLATED_PAWN_PENALTY;
        }

        // check if pawn is backward
        else if (isBackward(board, color, square))
        {
            openingScore -= BACKWARD_PAWN_PENALTY;
        }
        
        pawns &= pawns - 1;
    }
}

// king
int kingPawnShieldScore(Board& board, Color color, Square square)
{
    return popCount(kingSafetyArea(color, square) & board.getPiece(color, PAWN));
}

int kingPawnStormScore(Board& board, Color color, Square square)
{
    return popCount(kingDangerArea(color, square) & board.getPiece(static_cast<Color>(1 - color), PAWN));
}

UInt64 kingSafetyArea(Color color, Square square)
{
    UInt64 safetyArea = KING_ATTACKS[square];
    int rank = square / 8;

    // add rank 2 ranks ahead of king
    if (color == WHITE)
    {
        if (rank < 6) safetyArea |= (RANK_MASKS[rank + 2] & NEIGHBORING_FILES[square % 8]);
        if (rank > 0) safetyArea &= ~RANK_MASKS[rank - 1];
    }
    else
    {
        if (rank > 1) safetyArea |= (RANK_MASKS[rank - 2] & NEIGHBORING_FILES[square % 8]);
        if (rank < 7) safetyArea &= ~RANK_MASKS[rank + 1];
    }

    return safetyArea;
}

// king safety area + one extra rank in front of king
UInt64 kingDangerArea(Color color, Square square)
{
    UInt64 dangerArea = kingSafetyArea(color, square);
    int rank = square / 8;

    // add rank 3 ranks ahead of king
    if (color == WHITE)
    {
        if (rank < 5) dangerArea |= (RANK_MASKS[rank + 3] & NEIGHBORING_FILES[square % 8]);
    }
    else
    {
        if (rank > 2) dangerArea |= (RANK_MASKS[rank - 3] & NEIGHBORING_FILES[square % 8]);
    }

    return dangerArea;
}

void kingScore(Board& board, Color color, int& openingScore, int& endgameScore)
{
    // get king square   
    Square kingIndex = static_cast<Square>(lsb(board.getPiece(color, KING)));

    // check for king pawn shield scaled by enemy material
    openingScore += kingPawnShieldScore(board, color, kingIndex) * PAWN_SHIELD * board.getMaterial(static_cast<Color>(1 - color)) / PAWN_SHIELD_DIVISOR;

    // penalty for being stormed by enemy pawns
    openingScore -= kingPawnStormScore(board, color, kingIndex) * PAWN_STORM * board.getMaterial(static_cast<Color>(1 - color)) / PAWN_STORM_DIVISOR;

    // check for king on open file
    int file = kingIndex % 8;
    if (openFile(board, color, file))
    {
        openingScore -= KING_OPEN_FILE_PENALTY;
    }
    else if (halfOpenFile(board, color, file))
    {
        openingScore -= KING_HALF_OPEN_FILE_PENALTY;
    }

    // check for king next to open file
    if (file == 0)
    {
        if (openFile(board, color, 1))
        {
            openingScore -= KING_NEXT_TO_OPEN_FILE_PENALTY;
        }
        else if (halfOpenFile(board, color, 1))
        {
            openingScore -= KING_NEXT_TO_HALF_OPEN_FILE_PENALTY;
        }
    }
    else if (file == 7)
    {
        if (openFile(board, color, 6))
        {
            openingScore -= KING_NEXT_TO_OPEN_FILE_PENALTY;
        }
        else if (halfOpenFile(board, color, 6))
        {
            openingScore -= KING_NEXT_TO_HALF_OPEN_FILE_PENALTY;
        }
    }
    else
    {
        if (openFile(board, color, file - 1))
        {
            openingScore -= KING_NEXT_TO_OPEN_FILE_PENALTY;
        }
        else if (halfOpenFile(board, color, file - 1))
        {
            openingScore -= KING_NEXT_TO_HALF_OPEN_FILE_PENALTY;
        }

        if (openFile(board, color, file + 1))
        {
            openingScore -= KING_NEXT_TO_OPEN_FILE_PENALTY;
        }
        else if (halfOpenFile(board, color, file + 1))
        {
            openingScore -= KING_NEXT_TO_HALF_OPEN_FILE_PENALTY;
        }
    }
}
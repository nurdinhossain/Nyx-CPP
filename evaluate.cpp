#include "evaluate.h"
#include "bitboard.h"
#include <iostream>

// lazy evaluation
int lazyEvaluate(Board& board)
{
    int phase = board.getPhase();

    // material
    int openingScore = board.getMaterial(WHITE) - board.getMaterial(BLACK);

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
int evaluate(Board& board, PawnTable* pawnTable)
{
    int phase = board.getPhase();

    // material
    int openingScore = board.getMaterial(WHITE) - board.getMaterial(BLACK);

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
        else if (halfOpenFile(board, color, file))
        {
            openingScore += ROOK_HALF_OPEN_FILE;
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

            // check if pawn is protected
            if (isProtected(board, color, square))
            {
                openingScore += PROTECTED_PASSED_PAWN;
                endgameScore += PROTECTED_PASSED_PAWN;
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
            endgameScore -= BACKWARD_PAWN_PENALTY;
        }

        // check if pawn is doubled
        if (isDoubled(board, color, square))
        {
            openingScore -= DOUBLED_PAWN_PENALTY / 2;
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
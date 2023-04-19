#pragma once 

#include <string>
using UInt64 = unsigned long long;

enum Color
{
    WHITE,
    BLACK
};

enum Piece
{
    EMPTY,
    PAWN,
    KNIGHT,
    BISHOP,
    ROOK,
    QUEEN,
    KING
};

enum MoveType
{
    QUIET,
    DOUBLE_PAWN_PUSH,
    KING_CASTLE,
    QUEEN_CASTLE,
    CAPTURE,
    EN_PASSANT,
    KNIGHT_PROMOTION,
    BISHOP_PROMOTION,
    ROOK_PROMOTION,
    QUEEN_PROMOTION,
    KNIGHT_PROMOTION_CAPTURE,
    BISHOP_PROMOTION_CAPTURE,
    ROOK_PROMOTION_CAPTURE,
    QUEEN_PROMOTION_CAPTURE
};

enum Square 
{
    h1, g1, f1, e1, d1, c1, b1, a1,
    h2, g2, f2, e2, d2, c2, b2, a2,
    h3, g3, f3, e3, d3, c3, b3, a3,
    h4, g4, f4, e4, d4, c4, b4, a4,
    h5, g5, f5, e5, d5, c5, b5, a5,
    h6, g6, f6, e6, d6, c6, b6, a6,
    h7, g7, f7, e7, d7, c7, b7, a7,
    h8, g8, f8, e8, d8, c8, b8, a8, NONE
};

enum Castle 
{
    WHITE_KING_SIDE = 1,
    WHITE_QUEEN_SIDE = 2,
    BLACK_KING_SIDE = 4,
    BLACK_QUEEN_SIDE = 8
};

enum CastleThreat 
{
    WHITE_KINGSIDE_THREAT = 6,
    WHITE_QUEENSIDE_THREAT = 112,
    BLACK_KINGSIDE_THREAT = 432345564227567616,
    BLACK_QUEENSIDE_THREAT = 8070450532247928832
};

enum Phase
{
    PAWN_PHASE = 0,
    KNIGHT_PHASE = 1,
    BISHOP_PHASE = 1,
    ROOK_PHASE = 2,
    QUEEN_PHASE = 4,
    TOTAL_PHASE = 24
};

struct Move
{
    MoveType type;
    Square from;
    Square to;
    Piece pieceTaken;
    int oldCastle;
    Square oldEnPassant;
    int score;
};

constexpr UInt64 FILLED_BOARD = 0xFFFFFFFFFFFFFFFFULL;
constexpr int PIECE_VALUES[5] = {100, 310, 320, 500, 900};

/* PIECE-SQUARE TABLES */
constexpr int PAWN_TABLE_BLACK_OPENING[64] =
{
    0,  0,  0,  0,  0,  0,  0,  0,
    40, 40, 40, 40, 40, 40, 40, 40,
    10, 10, 15, 20, 20, 15, 10, 10,
    5,  5, 8, 17, 17, 8,  5,  5,
    0,  0,  0, 15, 15,  0,  0,  0,
    5, -5,-10,  0,  0,-10, -5,  5,
    5,  8,  8,-20,-20,  8,  8,  5,
    0,  0,  0,  0,  0,  0,  0,  0
};

constexpr int PAWN_TABLE_BLACK_ENDGAME[64] = 
{
    0, 0, 0, 0, 0, 0, 0, 0,
    60, 60, 60, 60, 60, 60, 60, 60,
    40, 40, 40, 40, 40, 40, 40, 40,
    30, 30, 30, 30, 30, 30, 30, 30,
    20, 20, 20, 20, 20, 20, 20, 20,
    10, 10, 10, 10, 10, 10, 10, 10,
    -10, -10, -10, -10, -10, -10, -10, -10,
    0, 0, 0, 0, 0, 0, 0, 0
};

constexpr int PAWN_TABLE_WHITE_OPENING[64] = 
{
    0,  0,  0,  0,  0,  0,  0,  0,
    5,  8,  8,-20,-20,  8,  8,  5,
    5, -5,-10,  0,  0,-10, -5,  5,
    0,  0,  0, 15, 15,  0,  0,  0,
    5,  5, 8, 17, 17, 8,  5,  5,
    10, 10, 15, 20, 20, 15, 10, 10,
    40, 40, 40, 40, 40, 40, 40, 40,
    0,  0,  0,  0,  0,  0,  0,  0
};

constexpr int PAWN_TABLE_WHITE_ENDGAME[64] = 
{
    0, 0, 0, 0, 0, 0, 0, 0,
    -10, -10, -10, -10, -10, -10, -10, -10,
    10, 10, 10, 10, 10, 10, 10, 10,
    20, 20, 20, 20, 20, 20, 20, 20,
    30, 30, 30, 30, 30, 30, 30, 30,
    40, 40, 40, 40, 40, 40, 40, 40,
    60, 60, 60, 60, 60, 60, 60, 60,
    0, 0, 0, 0, 0, 0, 0, 0
};

constexpr int KNIGHT_TABLE_BLACK_OPENING[64] =
{
    -25,-20,-15,-15,-15,-15,-20,-25,
    -20,-10,  0,  0,  0,  0,-10,-20,
    -15,  0, 10, 15, 15, 10,  0,-15,
    -15,  5, 15, 20, 20, 15,  5,-15,
    -15,  0, 15, 20, 20, 15,  0,-15,
    -15,  5, 10, 15, 15, 10,  5,-15,
    -20,-10,  0,  5,  5,  0,-10,-20,
    -25,-20,-15,-15,-15,-15,-20,-25
};

constexpr int KNIGHT_TABLE_BLACK_ENDGAME[64] =
{
    -20,-15,-10,-10,-10,-10,-15,-20,
    -15, -5,  5,  5,  5,  5, -5,-15,
    -10,  5, 15, 20, 20, 15,  5,-10,
    -10,  5, 15, 20, 20, 15,  5,-10,
    -15,  0, 15, 20, 20, 15,  0,-15,
    -15,  0,  5, 10, 10,  5,  0,-15,
    -20,-10,-10,-10,-10,-10,-10,-20,
    -40,-20,-20,-20,-20,-20,-20,-40
};

constexpr int KNIGHT_TABLE_WHITE_OPENING[64] =
{
    -25,-20,-15,-15,-15,-15,-20,-25,
    -20,-10,  0,  5,  5,  0,-10,-20,
    -15,  5, 10, 15, 15, 10,  5,-15,
    -15,  0, 15, 20, 20, 15,  0,-15,
    -15,  5, 15, 20, 20, 15,  5,-15,
    -15,  0, 10, 15, 15, 10,  0,-15,
    -20,-10,  0,  0,  0,  0,-10,-20,
    -25,-20,-15,-15,-15,-15,-20,-25
};

constexpr int KNIGHT_TABLE_WHITE_ENDGAME[64] =
{
    -40,-20,-20,-20,-20,-20,-20,-40,
    -20,-10,-10,-10,-10,-10,-10,-20,
    -15,  0,  5, 10, 10,  5,  0,-15,
    -15,  0, 15, 20, 20, 15,  0,-15,
    -10,  5, 15, 20, 20, 15,  5,-10,
    -10,  5, 15, 20, 20, 15,  5,-10,
    -15, -5,  5,  5,  5,  5, -5,-15,
    -20,-15,-10,-10,-10,-10,-15,-20
};

constexpr int BISHOP_TABLE_BLACK_OPENING[64] =
{
    -20,-10,-10,-10,-10,-10,-10,-20,
    -10,  0,  0,  0,  0,  0,  0,-10,
    -10,  0,  5, 10, 10,  5,  0,-10,
    -10,  5,  5, 10, 10,  5,  5,-10,
    -10,  0, 10, 10, 10, 10,  0,-10,
    -10,  5,  5,  5,  5,  5,  5,-10,
    -10, 15,  0,  0,  0,  0, 15,-10,
    -20,-10,-10,-10,-10,-10,-10,-20
};

constexpr int BISHOP_TABLE_BLACK_ENDGAME[64] =
{
    -20,-10,-10,-10,-10,-10,-10,-20,
    -10,  5,  5,  5,  5,  5,  5,-10,
    -10,  5, 15, 15, 15, 15,  5,-10,
    -10,  5, 15, 20, 20, 15,  5,-10,
    -10,  5, 15, 20, 20, 15,  5,-10,
    -10,  5, 15, 15, 15, 15,  5,-10,
    -10,  5,  5,  5,  5,  5,  5,-10,
    -20,-10,-10,-10,-10,-10,-10,-20
};

constexpr int BISHOP_TABLE_WHITE_OPENING[64] =
{
    -20,-10,-10,-10,-10,-10,-10,-20,
    -10, 15,  0,  0,  0,  0, 15,-10,
    -10,  5,  5,  5,  5,  5,  5,-10,
    -10,  0, 10, 10, 10, 10,  0,-10,
    -10,  5,  5, 10, 10,  5,  5,-10,
    -10,  0,  5, 10, 10,  5,  0,-10,
    -10,  0,  0,  0,  0,  0,  0,-10,
    -20,-10,-10,-10,-10,-10,-10,-20
};

constexpr int BISHOP_TABLE_WHITE_ENDGAME[64] =
{
    -20,-10,-10,-10,-10,-10,-10,-20,
    -10,  5,  5,  5,  5,  5,  5,-10,
    -10,  5, 15, 15, 15, 15,  5,-10,
    -10,  5, 15, 20, 20, 15,  5,-10,
    -10,  5, 15, 20, 20, 15,  5,-10,
    -10,  5, 15, 15, 15, 15,  5,-10,
    -10,  5,  5,  5,  5,  5,  5,-10,
    -20,-10,-10,-10,-10,-10,-10,-20
};

constexpr int ROOK_TABLE_BLACK_OPENING[64] = 
{
    0,  0,  0,  0,  0,  0,  0,  0,
    5, 10, 10, 10, 10, 10, 10,  5,
    -5,  0,  0,  0,  0,  0,  0, -5,
    -5,  0,  0,  0,  0,  0,  0, -5,
    -5,  0,  0,  0,  0,  0,  0, -5,
    -5,  0,  0,  0,  0,  0,  0, -5,
    -5,  0,  0,  0,  0,  0,  0, -5,
    0,  0,  0,  5,  5,  0,  0,  0
};

constexpr int ROOK_TABLE_BLACK_ENDGAME[64] = 
{
    3, 3, 3, 3, 3, 3, 3, 3,
    2, 2, 3, 3, 3, 3, 2, 2,
    1, 1, 2, 2, 2, 2, 1, 1,
    1, 1, 2, 2, 2, 2, 1, 1,
    1, 1, 2, 2, 2, 2, 1, 1,
    1, 1, 2, 2, 2, 2, 1, 1,
    1, 1, 2, 2, 2, 2, 1, 1,
    1, 1, 2, 2, 2, 2, 1, 1
};

constexpr int ROOK_TABLE_WHITE_OPENING[64] = 
{
    0,  0,  0,  5,  5,  0,  0,  0,
    -5,  0,  0,  0,  0,  0,  0, -5,
    -5,  0,  0,  0,  0,  0,  0, -5,
    -5,  0,  0,  0,  0,  0,  0, -5,
    -5,  0,  0,  0,  0,  0,  0, -5,
    -5,  0,  0,  0,  0,  0,  0, -5,
    5, 10, 10, 10, 10, 10, 10,  5,
    0,  0,  0,  0,  0,  0,  0,  0
};

constexpr int ROOK_TABLE_WHITE_ENDGAME[64] = 
{
    1, 1, 2, 2, 2, 2, 1, 1,
    1, 1, 2, 2, 2, 2, 1, 1,
    1, 1, 2, 2, 2, 2, 1, 1,
    1, 1, 2, 2, 2, 2, 1, 1,
    1, 1, 2, 2, 2, 2, 1, 1,
    1, 1, 2, 2, 2, 2, 1, 1,
    2, 2, 3, 3, 3, 3, 2, 2,
    3, 3, 3, 3, 3, 3, 3, 3
};

constexpr int QUEEN_TABLE_BLACK_OPENING[64] = 
{
    -20,-20,-20,-20,-20,-20,-20,-20,
    -20,-10,-10,-10,-10,-10,-10,-20,
    -20,-10,-10,-10,-10,-10,-10,-20,
    -20,-10,-10,-10,-10,-10,-10,-20,
    -20,-10,-10,-10,-10,-10,-10,-20,
    -10, -5, -5, -5, -5, -5,  0,-10,
    -10,  0,  0,  0,  0,  5,  0,-10,
    -20,-10,-10, -5, 10,-10,-10,-20
};

constexpr int QUEEN_TABLE_BLACK_ENDGAME[64] =
{
    -10, -5, -5,  -5,  -5, -5, -5,-10,
     -5,  6,  6,   6,   6,  6,  6, -5,
     -5,  6,  8,   8,   8,  8,  6, -5,
     -5,  6,  8,  10,  10,  8,  6, -5,
     -5,  6,  8,  10,  10,  8,  6, -5,
     -5,  6,  8,   8,   8,  8,  6, -5,
     -5,  6,  6,   6,   6,  6,  6, -5,
    -10, -5, -5,  -5,  -5, -5, -5,-10
};

constexpr int QUEEN_TABLE_WHITE_OPENING[64] =
{
    -20,-10,-10, -5, 10,-10,-10,-20,
    -10,  0,  0,  0,  0,  5,  0,-10,
    -10, -5, -5, -5, -5, -5,  0,-10,
    -20,-10,-10,-10,-10,-10,-10,-20,
    -20,-10,-10,-10,-10,-10,-10,-20,
    -20,-10,-10,-10,-10,-10,-10,-20,
    -20,-10,-10,-10,-10,-10,-10,-20,
    -20,-20,-20,-20,-20,-20,-20,-20,
};

constexpr int QUEEN_TABLE_WHITE_ENDGAME[64] = 
{
    -10, -5, -5,  -5,  -5, -5, -5,-10,
     -5,  6,  6,   6,   6,  6,  6, -5,
     -5,  6,  8,   8,   8,  8,  6, -5,
     -5,  6,  8,  10,  10,  8,  6, -5,
     -5,  6,  8,  10,  10,  8,  6, -5,
     -5,  6,  8,   8,   8,  8,  6, -5,
     -5,  6,  6,   6,   6,  6,  6, -5,
    -10, -5, -5,  -5,  -5, -5, -5,-10
};

constexpr int KING_TABLE_BLACK_OPENING[64] = 
{
    -30,-40,-40,-50,-50,-40,-40,-30,
    -30,-40,-40,-50,-50,-40,-40,-30,
    -30,-40,-40,-50,-50,-40,-40,-30,
    -30,-40,-40,-50,-50,-40,-40,-30,
    -20,-30,-30,-40,-40,-30,-30,-20,
    -10,-20,-20,-20,-20,-20,-20,-10,
     15, 15, -5,-10,-10, -5, 15, 15,
     15, 30,-10,-15,-15, 30, 25, 15
};

constexpr int KING_TABLE_BLACK_ENDGAME[64] = 
{
    -20,-15,-15,-10,-10,-15,-15,-20,
    -15, -5,  5,  5,  5,  5, -5,-15,
    -15,  5, 10, 15, 15, 10,  5,-15,
    -15,  5, 15, 20, 20, 15,  5,-15,
    -15,  5, 15, 20, 20, 15,  5,-15,
    -15,  5, 10, 15, 15, 10,  5,-15,
    -15, -5,  5,  5,  5,  5, -5,-15,
    -20,-15,-15,-10,-10,-15,-15,-20
};

constexpr int KING_TABLE_WHITE_OPENING[64] = 
{
    15, 30,-10,-15,-15, 30, 25, 15,
    15, 15, -5,-10,-10, -5, 15, 15,
    -10,-20,-20,-20,-20,-20,-20,-10,
    -20,-30,-30,-40,-40,-30,-30,-20,
    -30,-40,-40,-50,-50,-40,-40,-30,
    -30,-40,-40,-50,-50,-40,-40,-30,
    -30,-40,-40,-50,-50,-40,-40,-30,
    -30,-40,-40,-50,-50,-40,-40,-30
};

constexpr int KING_TABLE_WHITE_ENDGAME[64] = 
{
    -20,-15,-15,-10,-10,-15,-15,-20,
    -15, -5,  5,  5,  5,  5, -5,-15,
    -15,  5, 10, 15, 15, 10,  5,-15,
    -15,  5, 15, 20, 20, 15,  5,-15,
    -15,  5, 15, 20, 20, 15,  5,-15,
    -15,  5, 10, 15, 15, 10,  5,-15,
    -15, -5,  5,  5,  5,  5, -5,-15,
    -20,-15,-15,-10,-10,-15,-15,-20
};

// create multi-dimensional array called TABLES that stores the above arrays in 2 x 6 x 2 x 64 array (2 colors, 6 pieces, 2 phases, 64 squares)
extern const int* TABLES[2][6][2];
const int HISTORY_SIZE = 2; // 1 mb = 1 * 1024 * 1024 bytes

Color extractColor(int);
Piece extractPiece(int);
Square squareToIndex(std::string);
std::string indexToSquare(Square);

class Board
{
    public:
        // constructors
        Board(std::string fen);
        Board() : Board("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1") {}
        ~Board();

        // getters
        std::string getFen() const;
        UInt64 getPiece(Color color, Piece piece) const;
        UInt64 getOccupied(Color color) const;
        UInt64 getFullOccupied() const;
        int getSquareToPiece(Square square) const;
        int getCastlingRights() const;
        Color getNextMove() const;
        Square getEnPassant() const;
        UInt64 getCurrentHash() const;
        UInt64 getPawnHash() const;
        int getHistory() const;
        int getPieceCount(Color color, Piece piece) const;
        int getMaterial(Color color) const;
        int getStaticEvalOpening() const;
        int getStaticEvalEndgame() const;
        int getPhase() const;
        bool insufficientMaterial(Color color) const;
        UInt64 getCheckers() const;

        // setters
        void togglePiece(Color color, Piece piece, Square square);

        // other functions
        void print() const;
        void zobristHash();
        UInt64 inCheck(UInt64 bishopAttackFromKing, UInt64 rookAttackFromKing) const;
        UInt64 inCheckFull() const;
        UInt64 getAttackersForSquare(Square square) const;
        UInt64 checkBlockMask(UInt64 bishopAttackFromKing, UInt64 rookAttackFromKing) const;
        UInt64 pinned(UInt64 bishopAttackFromKing, UInt64 rookAttackFromKing) const;
        UInt64 pinnedLegalMask(Square pinnedIndex) const;

        // move generation
        UInt64 pawnPushBoard(Square square) const;
        UInt64 pawnAttackBoard(Square square) const;
        UInt64 knightAttackBoard(Square square) const;
        UInt64 bishopAttackBoard(Square square) const;
        UInt64 rookAttackBoard(Square square) const;
        UInt64 queenAttackBoard(Square square) const;
        UInt64 kingAttackBoard(Square square) const;
        void moveGenerationSetup();
        void generateMoves(Move moves[], int& moveCount, bool attackOnly = false, bool includeChecksWithAttacks = false, bool quietOnly = false);
        bool isEPLegal(Square from, Square to);
        bool isKingMoveLegal(Square from, Square to);
        void makeQuietMove(Square from, Square to);
        void makeCaptureMove(Square from, Square to);
        void makeEPMove(Square from, Square to);
        void castleKingSide(Color color);
        void castleQueenSide(Color color);
        void makePromoMove(Square from, Square to, Piece promoPiece);
        void makePromoCaptureMove(Square from, Square to, Piece promoPiece);
        void makeMove(Move &move);
        void unmakeMove(Move &move);
        Square makeNullMove();
        void unmakeNullMove(Square oldEnPassant);
        int perft(int depth);
        
    private:
        // for board state
        UInt64 pieces[2][6]; // 2 colors, 6 pieces
        UInt64 occupied[2]; // 2 colors
        UInt64 fullOccupied; // all occupied squares
        int squareToPiece[64]; // maps square to piece

        // for move generation
        int castlingRights; // 4 bits for white king, white queen, black king, black queen
        Color nextMove; // 0 for white, 1 for black
        Square enPassant; // 0-63 for square, 64 for none

        // hashing
        UInt64 currentHash; // current hash
        UInt64 pawnHash; // hash for pawns
        int size_; // size of history table
        int* history; // dynamic history table

        // for generating legal moves
        Square kingIndices[2]; // 2 colors
        UInt64 pinnedPiecesMG; // pinned pieces for move generation
        UInt64 checkers; // checkers for move generation
        UInt64 occMG; // occupancy for move generation  
        UInt64 blockMaskMG; // block mask for move generation

        // for static evaluation
        int pieceCounts[2][6]; // 2 colors, 6 pieces
        int material[2]; // 2 colors
        int staticEvalOpening; // static evaluation for opening
        int staticEvalEndgame; // static evaluation for endgame
};

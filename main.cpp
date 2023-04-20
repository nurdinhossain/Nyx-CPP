#include "bitboard.h"
#include "game.h"
#include "search.h"
#include "evaluate.h"
#include "searchboost.h"
#include "book.h"
#include <iostream>
#include <string>
#include <chrono>
using namespace std;

int main()
{
	// r4k1r/1pP1npp1/1Pb4p/p6q/Q1p2p1b/B1P2N2/P2N2P1/R2K3R w - - 0 1 is a wonderful FEN string to test with
	// rn3rk1/pbppq1pp/1p2pb2/4N2Q/3PN3/3B4/PPP2PPP/R3K2R w KQ - 7 11 is a mate in 7 puzzle

	// get user input
	string input;
	cout << "Enter a FEN string: ";
	getline(cin, input);

	// initialize sliding attacks
	processBishopAttacks("bishopAttackTable.txt");
	processRookAttacks("rookAttackTable.txt");

	// initialize board
	Board board = Board(input);
	std::cout << "Board initialized" << std::endl;

	/*std::vector<std::vector<std::string>> games = processPGN("book_games.pgn", 10000);
	std::cout << "Games processed" << std::endl;

	std::vector<std::string> fens = processGame(games[1]);
	std::cout << "FENs processed" << std::endl;
	for (int i = 0; i < fens.size(); i++)
	{
		std::cout << fens[i] << std::endl;
	}*/

	// search
	AI ai = AI();

	// get best move
	Move bestMove = ai.getBestMove(board);

	// make move and print board
	board.makeMove(bestMove);
	board.print();

	return 0;
}
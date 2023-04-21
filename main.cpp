#include "bitboard.h"
#include "game.h"
#include "search.h"
#include "evaluate.h"
#include "searchboost.h"
#include "book.h"
#include "tuner.h"
#include <iostream>
#include <string>
#include <chrono>
#include <fstream>
using namespace std;

int main()
{
	// r4k1r/1pP1npp1/1Pb4p/p6q/Q1p2p1b/B1P2N2/P2N2P1/R2K3R w - - 0 1 is a wonderful FEN string to test with
	// rn3rk1/pbppq1pp/1p2pb2/4N2Q/3PN3/3B4/PPP2PPP/R3K2R w KQ - 7 11 is a mate in 7 puzzle

	// initialize sliding attacks
	processBishopAttacks("bishopAttackTable.txt");
	processRookAttacks("rookAttackTable.txt");

	// get user input
	/*string input;
	cout << "Enter a FEN string: ";
	getline(cin, input);*/

	// initialize board
	//Board board = Board(input);
	//std::cout << "Board initialized" << std::endl;

	// search
	/*AI ai = AI();

	// get best move
	Move bestMove = ai.getBestMove(board);

	// make move and print board
	board.makeMove(bestMove);
	board.print();*/

	// create file to write to
	/*ofstream file("positions.txt");

	// process PGN file into games
	std::vector<std::vector<std::string>> games = processPGN("book_games.pgn", 1000);
	std::cout << "Games processed" << std::endl;

	// process games into FENs
	for (int i = 0; i < games.size(); i++)
	{
		std::vector<std::string> fens = processGame(games[i]);

		// remove first 50% and last 25% of FENs
		if (fens.size() >= 10)
		{
			// remove first 50%
			fens.erase(fens.begin(), fens.begin() + fens.size() / 2);

			// remove last 25%
			fens.erase(fens.begin() + fens.size() * 3 / 4, fens.end());

			// write to file
			for (int j = 0; j < fens.size(); j++)
			{
				file << fens[j] << endl;
			}
		}

		// print progress
		if (i % 500 == 0)
		{
			std::cout << i << " games processed" << std::endl;
		}
	}

	// close file
	file.close();*/

	// tune (0.5 is the best k value)
	tune("positions.txt", 0.5);

	return 0;
}
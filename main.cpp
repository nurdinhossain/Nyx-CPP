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

	// create file to write to
	/*ofstream file("positions.txt");

	// process PGN file into games
	std::vector<std::vector<std::string>> games = processPGN("book_games.pgn", 10000);
	std::cout << "Games processed" << std::endl;

	// process games into FENs
	int total = 0;
	for (int i = 0; i < games.size(); i++)
	{
		std::vector<std::string> fens = processGame(games[i]);

		if (fens.size() > 6)
		{
			// remove first 5 fens and last fen
			fens.erase(fens.begin(), fens.begin() + 5);
			fens.pop_back();
	
			for (int k = 0; k < fens.size(); k++)
			{
				file << fens[k] << std::endl;
				total++;
			}
		}

		// print progress
		if (i % 500 == 0)
		{
			std::cout << i << " games processed" << std::endl;
		}

		// stop at 20000 positions
		if (total >= 20000)
		{
			std::cout << i << " games processed in total" << std::endl;
			break;
		}
	}

	// close file
	std::cout << total << std::endl;
	file.close();*/

	// load parameters
	loadParameters("parameters.txt");

	// tune (0.5 is the best k value)
	//tune("positions.txt", 0.5);

	// get user input
	string input;
	cout << "Enter a FEN string: ";
	getline(cin, input);

	// initialize board
	Board board = Board(input);

	// initialize master ai
	AI master = AI(false);

	// initialize transposition table
	TranspositionTable* tt = new TranspositionTable(256);

	// search
	Move bestMove = threadedSearch(master, board, tt);

	// make move and print board
	board.makeMove(bestMove);
	board.print();
	std::cout << board.getFen() << std::endl;

	// clear master search stats
	master.getSearchStats().clear();

	// delete transposition table
	delete tt;

	return 0;
}
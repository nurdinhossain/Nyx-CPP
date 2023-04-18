#include "bitboard.h"
#include "game.h"
#include "search.h"
#include "evaluate.h"
#include "searchboost.h"
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

	// initialize board
	Board board = Board(input);
	std::cout << "Board initialized" << std::endl;
	processBishopAttacks("bishopAttackTable.txt");
	processRookAttacks("rookAttackTable.txt");
	
	// perft with time
	for (int i = 1; i <= 6; i++)
	{
		auto start = std::chrono::high_resolution_clock::now();
		std::cout << "Perft " << i << ": " << board.perft(i) << std::endl;
		auto end = std::chrono::high_resolution_clock::now();

		std::chrono::duration<double> elapsed = end - start;
		std::cout << "Time: " << elapsed.count() << "s" << std::endl;
	}

	return 0;
}
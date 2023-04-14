#include "bitboard.h"
#include "game.h"
#include "search.h"
#include <iostream>
#include <chrono>
using namespace std;

int main()
{
	Board board = Board("6k1/5p2/6p1/8/7p/8/6PP/6K1 b - - 0 0");
	std::cout << "Board initialized" << std::endl;
	processBishopAttacks("bishopAttackTable.txt");
	processRookAttacks("rookAttackTable.txt");
	
	// play game between two engines
	AI ai = AI();
	while (true)
	{
		// get best move
		Move bestMove = ai.getBestMove(board);

		// make move
		board.makeMove(bestMove);

		// print board
		board.print();
	}

	// perft
	/*for (int i = 1; i <= 6; i++)
	{
		auto start = std::chrono::high_resolution_clock::now();
		cout << board.perft(i) << endl;
		auto end = std::chrono::high_resolution_clock::now();
		auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
		cout << "Time: " << duration.count() << " ms" << endl;
	}*/

	return 0;
}
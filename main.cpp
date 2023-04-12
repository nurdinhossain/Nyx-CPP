#include "bitboard.h"
#include "game.h"
#include "search.h"
#include <iostream>
#include <chrono>
using namespace std;

int main()
{
	Board board = Board("6k1/3b3r/1p1p4/p1n2p2/1PPNpP1q/P3Q1p1/1R1RB1P1/5K2 b - - 0 1");
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
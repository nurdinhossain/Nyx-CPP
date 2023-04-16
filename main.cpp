#include "bitboard.h"
#include "game.h"
#include "search.h"
#include "evaluate.h"
#include "searchboost.h"
#include <iostream>
using namespace std;

int main()
{
	Board board = Board("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
	std::cout << "Board initialized" << std::endl;
	processBishopAttacks("bishopAttackTable.txt");
	processRookAttacks("rookAttackTable.txt");

	std::cout << evaluate(board) << std::endl;
	
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

	return 0;
}
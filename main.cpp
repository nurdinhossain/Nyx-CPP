#include "bitboard.h"
#include "game.h"
#include "search.h"
#include "evaluate.h"
#include "searchboost.h"
#include <iostream>
#include <string>
using namespace std;

int main()
{
	// get user input
	string input;
	cout << "Enter a FEN string: ";
	getline(cin, input);

	// initialize board
	Board board = Board(input);
	std::cout << "Board initialized" << std::endl;
	processBishopAttacks("bishopAttackTable.txt");
	processRookAttacks("rookAttackTable.txt");

	std::cout << evaluate(board) << std::endl;
	
	// get best move
	AI ai = AI();
	Move bestMove = ai.getBestMove(board);

	// make move
	board.makeMove(bestMove);

	// print board
	board.print();

	return 0;
}
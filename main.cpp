#include "bitboard.h"
#include "game.h"
#include "search.h"
#include "evaluate.h"
#include "searchboost.h"
#include "book.h"
#include "tuner.h"
#include "tables.h"
#include <iostream>
#include <string>
#include <chrono>
#include <fstream>
#include <cstring>
#include "client.h"
using namespace std;

int main()
{
	// r4k1r/1pP1npp1/1Pb4p/p6q/Q1p2p1b/B1P2N2/P2N2P1/R2K3R w - - 0 1 is a wonderful FEN string to test with
	// rn3rk1/pbppq1pp/1p2pb2/4N2Q/3PN3/3B4/PPP2PPP/R3K2R w KQ - 7 11 is a mate in 7 puzzle

	// initialize sliding attacks
	processBishopAttacks("bishopAttackTable.txt");
	processRookAttacks("rookAttackTable.txt");

	// load parameters
	loadParameters("best_parameters.txt");
	srand(time(NULL));

	// get console input for fen
	string fen;
	cout << "Enter FEN: ";
	getline(cin, fen);
	Board board = Board(fen);
	AI master = AI();
	TranspositionTable* tt = new TranspositionTable(TT_SIZE);
	string buffer;
	Move move = threadedSearch(master, board, tt, -1, buffer);

	delete tt;

	// pso
	/*while (true)
	{
		// run pso
		loadParameters("best_parameters.txt");
		pso("positions.txt", 0.5, 25, 0.7, 0.2, 0.1, 1);
	}*/

	/*************
	* SOCKET SETUP
	*************/
    /*int sock = connectSocket();
	string buffer;
	thread listenThread = initListenThread(sock, buffer);*/

	/*************
	* MAIN PROGRAM
	*************/
	/*Board board = Board("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
	AI master = AI(false);
	TranspositionTable* tt = new TranspositionTable(TT_SIZE);
	bool enemyMoveMade = false;
	bool allyMoveMade = false;

	while (true)
	{
		// get input from server
		while (buffer == "") {}

		// process input

		// if input contains commas, its a move and we should split it
		if (buffer.find(",") != string::npos && !enemyMoveMade)
		{
			// hold until last character is 'E', 'q', 'r', 'b', or 'n'
			while (buffer[buffer.length() - 1] != 'E' && buffer[buffer.length() - 1] != 'q' && buffer[buffer.length() - 1] != 'r' && buffer[buffer.length() - 1] != 'b' && buffer[buffer.length() - 1] != 'n') {}
			std::string input = buffer.substr(0, buffer.length());

			// generate moves list
			Move moves[MAX_MOVES];
			board.moveGenerationSetup();
			int numMoves = 0;
			board.generateMoves(moves, numMoves);

			// process move
			std::cout << input << std::endl;
			int firstCommaIndex = input.find(",");
			int secondCommIndex = input.find(",", firstCommaIndex + 1);
			string fromIndexStr = input.substr(0, firstCommaIndex);
			string toIndexStr = input.substr(firstCommaIndex + 1, secondCommIndex - firstCommaIndex - 1);
			string promotionStr = input.substr(secondCommIndex + 1, input.length() - secondCommIndex - 1);
			Square fromIndex = static_cast<Square>(stoi(fromIndexStr));
			Square toIndex = static_cast<Square>(stoi(toIndexStr));
			std::cout << "From: " << indexToSquare(fromIndex) << " To: " << indexToSquare(toIndex) << " Promotion: " << promotionStr << std::endl;

			// find move in moves list (must take care of promotions)
			Move move;
			for (int i = 0; i < numMoves; i++)
			{
				// if promotionStr is not "NONE"
				if (promotionStr != "NONE")
				{
					if (moves[i].from == fromIndex && moves[i].to == toIndex)
					{
						if ((moves[i].type == KNIGHT_PROMOTION || moves[i].type == KNIGHT_PROMOTION_CAPTURE) && promotionStr == "n")
						{
							move = moves[i];
							break;
						}
						else if ((moves[i].type == BISHOP_PROMOTION || moves[i].type == BISHOP_PROMOTION_CAPTURE) && promotionStr == "b")
						{
							move = moves[i];
							break;
						}
						else if ((moves[i].type == ROOK_PROMOTION || moves[i].type == ROOK_PROMOTION_CAPTURE) && promotionStr == "r")
						{
							move = moves[i];
							break;
						}
						else if ((moves[i].type == QUEEN_PROMOTION || moves[i].type == QUEEN_PROMOTION_CAPTURE) && promotionStr == "q")
						{
							move = moves[i];
							break;
						}
					}
				}
				else
				{
					if (moves[i].from == fromIndex && moves[i].to == toIndex)
					{
						move = moves[i];
						break;
					}
				}
			}

			// make move
			board.makeMove(move);

			// send ok message
			string message = "ok";
			sendMessage(sock, message);

			// set enemyMoveMade to true  and allyMoveMade to false
			enemyMoveMade = true;
			allyMoveMade = false;
		}
		// otherwise if buffer contains the word "start" and ends with 'L', start the search
		else if (buffer.find("start") != string::npos && buffer[buffer.length() - 1] == 'L' && !allyMoveMade)
		{
			std::string input = buffer.substr(0, buffer.length());
			std::cout << input << std::endl;

			// extract time limit from format "start <time_limit_in_decimal>L"
			int spaceIndex = input.find(" ");
			int timeLimit = stoi(input.substr(spaceIndex + 1, input.length() - spaceIndex - 2));
			MAX_TIME = timeLimit;

			// if we have an exact score for this position, halve the time limit
			Entry* entry = tt->probe(board.getCurrentHash());
			UInt64 smpKey = entry->smpKey;
			UInt64 data = entry->data;
			if (board.getCurrentHash() == (smpKey ^ data))
			{
				if (((data >> 40) & 3) == EXACT)
					MAX_TIME /= 2;
			}

			// search
			board.print();
			Move move = threadedSearch(master, board, tt, sock, buffer);
			board.makeMove(move);
			master.getSearchStats().clear();

			// send "stop" to server
			string message = "stop";
			sendMessage(sock, message);

			// set allyMoveMade to true and enemyMoveMade to false
			allyMoveMade = true;
			enemyMoveMade = false;

			// set time back to 600
			MAX_TIME = 600;
		}
		else if (buffer.find("ponder") != string::npos && !enemyMoveMade)
		{
			// search
			Move move = threadedSearch(master, board, tt, sock, buffer);
			master.getSearchStats().clear();

			// send "ok" to server
			string message = "ok";
			sendMessage(sock, message);
		}
	}

	// delete transposition table
	delete tt;

	closeSocket(sock);
	killListenThread(listenThread);*/

	return 0;
}

// code for processing PGNs
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
	file.close();

	// display piece-square tables
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 2; j++)
		{
			for (int row = 0; row < 8; row++)
			{
				for (int col = 0; col < 8; col++)
				{
					std::cout << TABLES[i][j][row*8 + col] << "\t";
				}
				std::cout << std::endl;
			}
			std::cout << std::endl;
		}
		std::cout << std::endl;
	}

	// tune (0.5 is the best k value)
	//tune("positions.txt", 0.5);
	
	*/
#include "uci.h"
#include "game.h"
#include "search.h"
#include "transposition.h"
#include "book.h"
#include <iostream>
#include <thread>
#include <vector>

// method for uci protocol
void uci()
{
    Board board;
    AI master;
    TranspositionTable* tt = new TranspositionTable(TT_SIZE);
    bool DEBUG = false;

    while (true)
    {
        string buffer;
        getline(cin, buffer);
        if (buffer == "uci")
        {
            cout << "id name " << ENGINE_NAME << endl;
            cout << "id author " << ENGINE_AUTHOR << endl;
            cout << "uciok" << endl;
        }
        else if (buffer == "isready")
        {
            cout << "readyok" << endl;
        }
        // debug
        else if (buffer.substr(0, 5) == "debug")
        {
            if (buffer.substr(6, 3) == "on")
                DEBUG = true;
            else
                DEBUG = false;
        }
        else if (buffer == "ucinewgame")
        {
            // reset board
            board = Board("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");

            // reset transposition table
            tt->clear();

            // reset AI
            master = AI();
        }
        else if (buffer.substr(0, 8) == "position")
        {
            // position startpos moves e2e4 e7e5 b7b8q
            string fen = "";
            if (buffer.substr(9, 8) == "startpos")
            {
                fen = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
            }
            else
            {
                // position fen rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w
                // KQkq - 0 1 moves e2e4 e7e5 b7b8q
                // if does not contain moves, then it is just a fen
                if (buffer.find("moves") == string::npos)
                {
                    fen = buffer.substr(13, buffer.length() - 13);
                }
                else
                {
                    // if it does contain moves, then we can go until the word moves
                    fen = buffer.substr(13, buffer.find("moves") - 14);
                }
            }

            // process moves (split into vector of strings)
            vector<string> moves;
            if (buffer.find("moves") != string::npos)
            {
                string movesStr = buffer.substr(buffer.find("moves") + 6, buffer.length() - buffer.find("moves") - 6);
                string move = "";
                for (int i = 0; i < movesStr.length(); i++)
                {
                    if (movesStr[i] == ' ')
                    {
                        moves.push_back(move);
                        move = "";
                    }
                    else
                    {
                        move += movesStr[i];
                    }
                }
                moves.push_back(move);
            }

            // process moves if there are any
            vector<string> fens;
            if (moves.size() > 0)
                fens = processGame(moves, fen);

            // set board to last fen if there are any moves, otherwise just set it to the fen
            if (fens.size() > 0)
                board = Board(fens[fens.size() - 1]);
            else
                board = Board(fen);
        }
        else if (buffer.substr(0, 2) == "go")
        {
            // go wtime 300000 btime 300000 winc 0 binc 0

            // just search for 1 second
            MAX_TIME = 1;
            //threadedSearch(master, board, tt, -1, buffer);
        }
        else if (buffer == "stop")
        {
            // stop
        }
        else if (buffer == "quit")
        {
            // quit
            break;
        }
    }
}
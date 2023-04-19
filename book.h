#pragma once
#include "game.h"
#include <vector>
#include <string>
#include <unordered_map>

// method to read and preprocess a PGN file into separate games (represented by a vector of strings)
std::vector<std::vector<std::string>> processPGN(std::string filename, int limit=5000);

// method to search through a list of moves given a piece and seeing if a certain piece type can move to a certain square with different pieces
int ambiguousMove(Board& board, Move moves[], int moveCount, Piece piece, int square);

// method to convert a vector of strings (game) into a vector of fen strings
std::vector<std::string> processGame(std::vector<std::string> game);
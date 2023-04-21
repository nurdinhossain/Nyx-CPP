#include "tuner.h"
#include "game.h"
#include "search.h"
#include "evalparams.h"
#include "tables.h"
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <cmath>
using namespace std;

// given a txt file of FENs + game result, open the file and get mean squared error of quiesce()
float mse(std::string filename, float k)
{
    // open file and get lines
    ifstream file(filename);
    string line;
    vector<string> lines;
    vector<float> results;
    while (getline(file, line))
    {
        // split line into fen, result by comma
        string delimiter = ",";
        size_t pos = 0;
        string token;
        while ((pos = line.find(delimiter)) != string::npos)
        {
            // get token
            token = line.substr(0, pos);
            lines.push_back(token);
            line.erase(0, pos + delimiter.length());
        }
        results.push_back(stof(line));
    }

    // go through each line and get mse
    float total = 0;
    AI ai = AI();
    for (int i = 0; i < lines.size(); i++)
    {
        // get fen and result
        string fen = lines[i];
        float result = results[i];

        // get board
        Board board = Board(fen);

        // get quiesce score (relative to white)
        float score = ai.quiesce(board, NEG_INF, POS_INF) * (board.getNextMove() == WHITE ? 1 : -1);

        // map quiesce score to sigmoid using pow function
        float sigmoid = 1 / (1 + pow(10, -k * score / 400));

        // add to total
        total += pow(sigmoid - result, 2);
    }

    // close file
    file.close();

    // return mse
    return total / lines.size();
}

// find the best k value for tuning
void findBestK(std::string filename, float start, float end, float step)
{
    // initialize best k and mse
    float bestK = start;
    float bestMSE = 1000000;

    // go through each k value
    for (float k = start; k <= end; k += step)
    {
        // get mse
        float m = mse(filename, k);

        // if mse is less than best mse, update best mse and best k
        if (m < bestMSE)
        {
            bestMSE = m;
            bestK = k;
            std::cout << "Best k: " << bestK << std::endl;
        }

        // print k and mse
        cout << "k: " << k << endl;
        cout << "MSE: " << m << endl;
    }

    // print best k and mse
    cout << "Best k: " << bestK << endl;
    cout << "Best MSE: " << bestMSE << endl;
}

// vectorize parameters
std::vector<int*> vectorizeParameters()
{
    // initialize vector of parameters
    vector<int*> parameters;

    // add parameters to vector
    for (int piece = 0; piece < 6; piece++)
    {
        for (int phase = 0; phase < 2; phase++)
        {
            for (int square = 0; square < 64; square++)
            {
                parameters.push_back(&TABLES[piece][phase][square]);
            }
        }
    }

    parameters.push_back(&PASSED_PAWN);
    parameters.push_back(&PROTECTED_PASSED_PAWN);
    parameters.push_back(&OUTSIDE_PASSED_PAWN);
    parameters.push_back(&BACKWARD_PAWN_PENALTY);
    parameters.push_back(&ISOLATED_PAWN_PENALTY);
    parameters.push_back(&UNPROTECTED_PAWN_PENALTY);
    parameters.push_back(&DOUBLED_PAWN_PENALTY);

    parameters.push_back(&KNIGHT_DECREASE_WITH_PAWN_LOSS);
    parameters.push_back(&KNIGHT_OUTPOST);
    parameters.push_back(&KNIGHT_OUTPOST_ON_HOLE);

    parameters.push_back(&BISHOP_PAIR);
    parameters.push_back(&BISHOP_MOBILITY);

    parameters.push_back(&ROOK_INCREASE_WITH_PAWN_LOSS);
    parameters.push_back(&ROOK_OPEN_FILE);
    parameters.push_back(&ROOK_HALF_OPEN_FILE);

    parameters.push_back(&KING_BLOCK_ROOK_PENALTY);
    parameters.push_back(&KING_OPEN_FILE_PENALTY);
    parameters.push_back(&KING_HALF_OPEN_FILE_PENALTY);
    parameters.push_back(&KING_NEXT_TO_OPEN_FILE_PENALTY);
    parameters.push_back(&KING_NEXT_TO_HALF_OPEN_FILE_PENALTY);
    parameters.push_back(&PAWN_SHIELD);
    parameters.push_back(&PAWN_STORM);
    parameters.push_back(&PAWN_SHIELD_DIVISOR);
    parameters.push_back(&TEMPO_BONUS);

    return parameters;
}   

// copy vector parameters to pointer parameters
void copyParametersToPointers(std::vector<int> parameters, std::vector<int*> pointers)
{
    for (int i = 0; i < parameters.size(); i++)
    {
        *pointers[i] = parameters[i];
    }
}

// get parameters from pointers
std::vector<int> getParametersFromPointers(std::vector<int*> pointers)
{
    vector<int> parameters;
    for (int i = 0; i < pointers.size(); i++)
    {
        parameters.push_back(*pointers[i]);
    }
    return parameters;
}

// texel tuning
void tune(std::string filename, float k)
{
    // get parameters
    vector<int*> parameters = vectorizeParameters();

    // initialize best mse
    float bestMSE = 1000000;

    // improved flag
    bool improved = true;

    while (improved)
    {
        improved = false;

        // go through each parameter
        for (int i = 0; i < parameters.size(); i++)
        {
            // get parameter
            int* parameter = parameters[i];

            // get original parameter
            int originalParameter = *parameter;

            // get original mse
            float originalMSE = mse(filename, k);

            // increase parameter
            *parameter = originalParameter + 1;
            float increasedMSE = mse(filename, k);

            // decrease parameter
            *parameter = originalParameter - 1;
            float decreasedMSE = mse(filename, k);

            // reset parameter
            *parameter = originalParameter;

            // if increased mse is less than original mse, increase parameter
            if (increasedMSE < originalMSE)
            {
                *parameter = originalParameter + 1;
                improved = true;
            }

            // if decreased mse is less than original mse, decrease parameter
            else if (decreasedMSE < originalMSE)
            {
                *parameter = originalParameter - 1;
                improved = true;
            }

            // if increased mse is less than decreased mse, increase parameter
            else if (increasedMSE < decreasedMSE)
            {
                *parameter = originalParameter + 1;
                improved = true;
            }

            // if decreased mse is less than increased mse, decrease parameter
            else if (decreasedMSE < increasedMSE)
            {
                *parameter = originalParameter - 1;
                improved = true;
            }

            // if mse is less than best mse, update best mse and best parameters
            if (originalMSE < bestMSE)
            {
                bestMSE = originalMSE;
            }

            // print parameter, original mse, increased mse, decreased mse, and best mse
            cout << "Parameter: " << i << endl;
            cout << "Original MSE: " << originalMSE << endl;
            cout << "Increased MSE: " << increasedMSE << endl;
            cout << "Decreased MSE: " << decreasedMSE << endl;
            cout << "Best MSE: " << bestMSE << endl;
        }
    }

    // output parameters to file separated by commas
    ofstream outputFile;
    outputFile.open("parameters.txt");
    vector<int> bestParameters = getParametersFromPointers(parameters);
    for (int i = 0; i < bestParameters.size(); i++)
    {
        outputFile << bestParameters[i];
        if (i != bestParameters.size() - 1)
        {
            outputFile << ",";
        }
    }
    outputFile.close();
}
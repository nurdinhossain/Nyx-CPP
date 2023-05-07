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

// process file of FENs
void processFENs(string filename, vector<string>& lines, vector<float>& results)
{
    // open file and get lines
    ifstream file(filename);
    string line;
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

        // print progress
        if (lines.size() % 10000 == 0)
        {
            cout << lines.size() << " lines processed" << endl;
        }
    } 

    // close the file 
    file.close();
}

// given a txt file of FENs + game result, open the file and get mean squared error of quiesce()
float mse(vector<string>& lines, vector<float>& results, float k)
{
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
    
    // return mse
    return total / lines.size();
}

// find the best k value for tuning
/*void findBestK(std::string filename, float start, float end, float step)
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
}*/

// vectorize parameters
std::vector<int*> vectorizeParameters()
{
    // initialize vector of parameters
    vector<int*> parameters;

    // add parameters to vector
    /*for (int piece = 0; piece < 6; piece++)
    {
        for (int phase = 0; phase < 2; phase++)
        {
            for (int square = 0; square < 64; square++)
            {
                parameters.push_back(&TABLES[piece][phase][square]);
            }
        }
    }*/

    parameters.push_back(&PASSED_PAWN);
    parameters.push_back(&UNSTOPPABLE_PASSED_PAWN);
    parameters.push_back(&CANDIDATE_PASSED_PAWN);
    parameters.push_back(&UNOBSTRUCTED_BONUS);
    parameters.push_back(&BACKWARD_PAWN_PENALTY);
    parameters.push_back(&ISOLATED_PAWN_PENALTY);

    parameters.push_back(&KNIGHT_OUTPOST);
    parameters.push_back(&KNIGHT_OUTPOST_ON_HOLE);

    parameters.push_back(&ROOK_OPEN_FILE);

    parameters.push_back(&KING_BLOCK_ROOK_PENALTY);
    parameters.push_back(&SAFETY_TABLE_MULTIPLIER);
    parameters.push_back(&MINOR_ATTACK_UNITS);
    parameters.push_back(&ROOK_ATTACK_UNITS);
    parameters.push_back(&QUEEN_ATTACK_UNITS);
    parameters.push_back(&ROOK_CHECK_UNITS);
    parameters.push_back(&QUEEN_CHECK_UNITS);
    parameters.push_back(&PAWN_SHIELD);
    parameters.push_back(&PAWN_STORM);
    parameters.push_back(&PAWN_SHIELD_DIVISOR);
    parameters.push_back(&PAWN_STORM_DIVISOR);

    return parameters;
}   

void loadParameters(string filename)
{
    ifstream file(filename);
    string line;
    int i = 0;
    vector<int*> parameters = vectorizeParameters();

    // split comma-separated values
    while (getline(file, line))
    {
        string delimiter = ",";
        size_t pos = 0;
        string token;
        while ((pos = line.find(delimiter)) != string::npos)
        {
            // get token
            token = line.substr(0, pos);
            *parameters[i] = stoi(token);
            line.erase(0, pos + delimiter.length());
            i++;
        }
        *parameters[i] = stoi(line);
        i++;
    }
}

// copy vector parameters to pointer parameters
void copyParametersToPointers(vector<int> parameters, vector<int*> pointers)
{
    for (int i = 0; i < parameters.size(); i++)
    {
        *pointers[i] = parameters[i];
    }
}

// get parameters from pointers
std::vector<int> getParametersFromPointers(vector<int*> pointers)
{
    vector<int> parameters;
    for (int i = 0; i < pointers.size(); i++)
    {
        parameters.push_back(*pointers[i]);
    }
    return parameters;
}

// texel tuning
void tune(string filename, float k)
{
    // get parameters
    vector<int*> parameters = vectorizeParameters();

    // get lines and results
    vector<string> lines;
    vector<float> results;
    processFENs(filename, lines, results);

    // initialize best mse
    float bestMSE = mse(lines, results, k);

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

            // increase parameter
            *parameter = originalParameter + 1;
            float increasedMSE = mse(lines, results, k);

            // decrease parameter
            *parameter = originalParameter - 1;
            float decreasedMSE = mse(lines, results, k);

            // reset parameter
            *parameter = originalParameter;

            // update parameter if improved
            if (increasedMSE < bestMSE && decreasedMSE < bestMSE)
            {
                if (increasedMSE < decreasedMSE)
                {
                    *parameter = originalParameter + 1;
                }
                else
                {
                    *parameter = originalParameter - 1;
                }
                improved = true;
            }
            else if (increasedMSE < bestMSE)
            {
                *parameter = originalParameter + 1;
                improved = true;
            }
            else if (decreasedMSE < bestMSE)
            {
                *parameter = originalParameter - 1;
                improved = true;
            }

            // update best mse
            bestMSE = min(bestMSE, min(increasedMSE, decreasedMSE));

            // print parameter and mses
            cout << "Parameter: " << i << endl;
            cout << "Original MSE: " << bestMSE << endl;
            cout << "Increased MSE: " << increasedMSE << endl;
            cout << "Decreased MSE: " << decreasedMSE << endl;
        }

        // save parameters every iteration
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
}
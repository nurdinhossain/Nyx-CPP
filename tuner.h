#pragma once 
#include <string>
#include <vector>
using namespace std;

// process file of FENs
void processFENs(string filename, vector<string>& lines, vector<float>& results);

// given a txt file of FENs + game result, open the file and get mean squared error of quiesce()
float mse(vector<string>& lines, vector<float>& results, float k);

// find the best k value for tuning
void findBestK(string filename, float start, float end, float step);

// vectorize parameters
vector<int*> vectorizeParameters();

// vectorize second half of first half of tables
vector<int*> vectorizeTablesSecondHalf();

// load in parameters from file
void loadParameters(string filename);

// copy vector parameters to pointer parameters
void copyParametersToPointers(vector<int> parameters, vector<int*> pointers);

// get parameters from pointers
vector<int> getParametersFromPointers(vector<int*> pointers);

// texel tuning
void tune(string filename, float k);

// save parameters to file
void saveParameters(string filename, vector<int*> parameters);

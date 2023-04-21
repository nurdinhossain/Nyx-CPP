#pragma once 
#include <string>
#include <vector>

// given a txt file of FENs + game result, open the file and get mean squared error of quiesce()
float mse(std::string filename, float k);

// find the best k value for tuning
void findBestK(std::string filename, float start, float end, float step);

// vectorize parameters
std::vector<int*> vectorizeParameters();

// copy vector parameters to pointer parameters
void copyParametersToPointers(std::vector<int> parameters, std::vector<int*> pointers);

// get parameters from pointers
std::vector<int> getParametersFromPointers(std::vector<int*> pointers);

// texel tuning
void tune(std::string filename, float k);

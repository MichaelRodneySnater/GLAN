#include "SceneParser.hpp"
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>

SceneParser::SceneParser()
: fileName_("SCENE_DATA/scene.csv")
, costMatrix_()
, numMatricies_(0)
{
}

SceneParser::~SceneParser()
{}

void SceneParser::ReadInCsv(void)
{
    std::ifstream file(fileName_);
    if(!file.is_open())
    {
        std::cerr << "Error: Failed to open CSV: " << fileName_ << std::endl;
        return;
    }
    std::string line;

    int matrixNumber{-1};
    while(std::getline(file,line))
    {
        std::stringstream ss(line);
        std::string token;
        std::vector<std::string> tokens;

        // Split the line by comma
        while(std::getline(ss,token,','))
        {
            tokens.push_back(token);
        }
        
        // Id when new frame data is available
        if (std::stoi(tokens[0]) != matrixNumber)
        {
            matrixNumber = std::stoi(tokens[0]);
        }

        if (tokens[1] == "detection")
        {
        }
    }

}
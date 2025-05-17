#include "SceneParser.hpp"
#include "Tracker.hpp"
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>

SceneParser::SceneParser()
: fileName_("SCENE_DATA/scene.csv")
, frameData_(MAX_FRAMES)
, numberFrames_(0)
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

    int frameNumber{-1};
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
        if (std::stoi(tokens[0]) != frameNumber)
        {
            frameNumber = std::stoi(tokens[0]);
        }

        if (tokens[1] == "detection")
        {
            frameData_[frameNumber].detList_[std::stoi(tokens[2])].pos[0] = std::stof(tokens[3]);
            frameData_[frameNumber].detList_[std::stoi(tokens[2])].pos[1] = std::stof(tokens[4]);
        }
        if (tokens[1] == "track")
        {
            frameData_[frameNumber].trackList_[std::stoi(tokens[2])].predState(0) = std::stof(tokens[3]);
            frameData_[frameNumber].trackList_[std::stoi(tokens[2])].predState(1) = std::stof(tokens[4]);
        }
        if (tokens[1] == "truthAss")
        {   
            auto &truth = frameData_[frameNumber].truthAssingment_;
            truth.clear();
            truth.reserve(tokens.size()-2);

            for (size_t i = 2; i < tokens.size();++i)
            {
                truth.push_back(std::stoi(tokens[i]));
            }
            std::cout << "Size of truthAss: " << truth.size()<<std::endl;
        }

    }

}
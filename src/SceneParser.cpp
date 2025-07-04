#include "SceneParser.hpp"
#include "Utilties.hpp"

SceneParser::SceneParser(   std::vector<std::vector<std::vector<double>>>& costMatrix,
                            std::vector<int>& numTracks,
                            std::vector<int>& numDets)
: fileName_("SCENE_DATA/matricies.csv")
, costMatrix_(costMatrix)
, numTracks_(numTracks)
, numDets_(numDets)
{
}

SceneParser::~SceneParser()
{
    // delete costMatrix_;
    // delete numTracks_;
    // delete numDets_;
}

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
    // [MATRIX][ROWS][COLS]
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
    
        if (tokens[0] == "matrixNumber")
        {
            matrixNumber = std::stoi(tokens[1]);
            numTracks_.push_back(std::stoi(tokens[2]));
            numDets_.push_back(std::stoi(tokens[3]));

            
        }

        if (matrixNumber >= costMatrix_.size())
        {
            costMatrix_.resize(matrixNumber+1);
        }

        if (tokens[0] == "row")
        {
            int row = std::stoi(tokens[1]);

            if (row >= costMatrix_[matrixNumber].size())
            {
                costMatrix_[matrixNumber].resize(row+1);
            }

            std::vector<double> rowVec;
            for (size_t ii = 2; ii < tokens.size(); ++ii)
            {
                rowVec.push_back(std::stof(tokens[ii]));
            }

            costMatrix_[matrixNumber][row] = rowVec;
            
        }
    }

}
# pragma once
#include <string>
#include <vector>

class SceneParser {
    public:
        SceneParser(std::vector<std::vector<std::vector<double>>>& costMatrix);
        ~SceneParser();
        void ReadInCsv(void);
    
        std::vector<std::vector<std::vector<double>>>& getCostMatrix() {return costMatrix_;} 

    private:

        std::string fileName_;
        // [MATRIX][ROWS][COLS]
        std::vector<std::vector<std::vector<double>>>& costMatrix_;
        int numMatricies_;
        const int MAX_MATRICIES = 1000;
};
# pragma once
#include "Utilties.hpp"

class SceneParser {
    public:
        SceneParser(std::vector<std::vector<std::vector<double>>>& costMatrix,
                    std::vector<int>& numTracks,
                    std::vector<int>& numDets);
        ~SceneParser();
        void ReadInCsv(void);
    
        std::vector<std::vector<std::vector<double>>>& getCostMatrix(void) {return costMatrix_;} 
        std::vector<int>& getNumTracks(void) {return numTracks_;}
        std::vector<int>& getNumDets(void) {return numDets_;}

    private:

        std::string fileName_;
        // [MATRIX][ROWS][COLS]
        std::vector<std::vector<std::vector<double>>>& costMatrix_;
        std::vector<int>& numTracks_;
        std::vector<int>& numDets_;
        int numMatricies_;
};
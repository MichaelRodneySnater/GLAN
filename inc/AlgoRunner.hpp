// This needs:
//      1. Read in const matrix
//      2. Selecter for LAP algos
//      3. A bunch of different LAP algos
//      4. Hungarian to operate as "TRUTH"
//      5. Method of storing output truth ass for each algo.
//      6. Method of comparing all algos against eachother.
#pragma once
#include "Utilties.hpp"

class AlgoRunner
{
private:
    
public:
    AlgoRunner();
    ~AlgoRunner();

    void setCostMatrix(const std::vector<std::vector<double>>& costMatrix);
    void setNumTracks(const int& numTracks);
    void setNumDets(const int& numDets);
    void populateAssignmentArray(std::vector<int>& assignmentArray); 
    void setAlgorithm(const Util::ALGORITHM alg);
    void run(void);
    void runHungarian(void);
    void print_assignment(void) const;

    Util::ALGORITHM lapAlgo_;
    std::vector<int> assignment_;
    const std::vector<std::vector<double>>* costMatrix_;
    const int* numTracks_;
    const int* numDets_;


    // CONSTS
    const double INF = std::numeric_limits<double>::infinity();

};
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

    void setAlgorithm(const Util::ALGORITHM alg);

    Util::ALGORITHM lapAlgo_;
    std::vector<int> assignment_;

};
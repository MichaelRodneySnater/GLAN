#include "AlgoRunner.hpp"
#include "Utilties.hpp"

AlgoRunner::AlgoRunner()
: lapAlgo_(Util::HUNGARIAN)
{
}

AlgoRunner::~AlgoRunner()
{
}

void AlgoRunner::setAlgorithm(const Util::ALGORITHM alg)
{
    lapAlgo_ = alg;
}

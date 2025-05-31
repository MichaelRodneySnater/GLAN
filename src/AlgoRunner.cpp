#include "AlgoRunner.hpp"
#include "Utilties.hpp"

AlgoRunner::AlgoRunner()
: lapAlgo_(Util::HUNGARIAN)
, costMatrix_(nullptr)
, numTracks_(nullptr)
, numDets_(nullptr)
{
}

AlgoRunner::~AlgoRunner()
{
}

void AlgoRunner::setCostMatrix(const std::vector<std::vector<double>>& costMatrix)
{
    costMatrix_ = &costMatrix;
}

void AlgoRunner::setNumTracks(const int& numTracks)
{
    numTracks_ = &numTracks;
}

void AlgoRunner::setNumDets(const int& numDets)
{
    numDets_ = &numDets;
}

void AlgoRunner::setAlgorithm(const Util::ALGORITHM alg)
{
    lapAlgo_ = alg;
}

void AlgoRunner::run(void)
{
    switch(lapAlgo_)
    {
        case Util::HUNGARIAN:
            runHungarian();
            break;
        case Util::JV:
            // runJV();
            break;
        case Util::AUCTION:
            // runAuction();
            break;
        case Util::BINNING:
            // runBinng();
            break;
        default:
            throw std::invalid_argument("AlgoRunner::run::swtich(lapAlgo_) enumeration not in swtich statement.");
            break;
    }
}

void AlgoRunner::print_assignment(void) const
{
    double totalCost = 0;
    for (int i = 0; i < assignment_.size(); ++i) {
        std::cout << "Row " << i << " assigned to Column " << assignment_[i]
             << " (Cost: " << (*costMatrix_)[i][assignment_[i]] << ")\n";
        totalCost += (*costMatrix_)[i][assignment_[i]];
    }
    std::cout << std::fixed << std::setprecision(6) << "Total Cost: " << totalCost << std::endl;
}

void AlgoRunner::runHungarian(void)
{
    // Init the class stuff
    int n = std::max(costMatrix_->size(), costMatrix_[0].size());
    assignment_.clear();
    assignment_.resize(n, -1);
    std::vector<double> u(n + 1), v(n + 1);
    std::vector<int> p(n + 1), way(n + 1);

    for (int i = 1; i <= n; ++i) {
        p[0] = i;
        std::vector<double> minv(n + 1, INF);
        std::vector<bool> used(n + 1, false);
        int j0 = 0;

        do {
            used[j0] = true;
            int i0 = p[j0];
            double delta = INF;
            int j1 = -1;

            for (int j = 1; j <= n; ++j) {
                if (!used[j]) {
                    double cur = (*costMatrix_)[i0 - 1][j - 1] - u[i0] - v[j];
                    if (cur < minv[j]) {
                        minv[j] = cur;
                        way[j] = j0;
                    }
                    if (minv[j] < delta) {
                        delta = minv[j];
                        j1 = j;
                    }
                }
            }

            for (int j = 0; j <= n; ++j) {
                if (used[j]) {
                    u[p[j]] += delta;
                    v[j] -= delta;
                } else {
                    minv[j] -= delta;
                }
            }

            j0 = j1;
        } while (p[j0] != 0);

        do {
            int j1 = way[j0];
            p[j0] = p[j1];
            j0 = j1;
        } while (j0);
    }
    
    // Populate the assignment array
    for (int j = 1; j <= n; ++j)
    {
        if (j >= *numTracks_)
        {
            assignment_[p[j] - 1] = -1; // Assign the real tracks to -1
            assignment_[j] = -1; // assign the tracks that represent a det being a false alarm
        }
        else
        {
            // Track assigned to detections
            assignment_[p[j] - 1] = j - 1; // proper associaiton
        }
    }

}

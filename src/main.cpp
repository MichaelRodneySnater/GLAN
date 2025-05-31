#include "SceneParser.hpp"
#include "AlgoRunner.hpp"
#include "Utilties.hpp"

int main(void)
{
    // Populate Cost Matrix using csv parser class
    std::vector<std::vector<std::vector<double>>> costMatrix;
    std::vector<int> numTracks;
    std::vector<int> numDets;
    std::vector<int> assignmentArray;
    std::unique_ptr<SceneParser> sceneReader = std::make_unique<SceneParser>(costMatrix, numTracks, numDets);
    sceneReader->ReadInCsv();

    // Run the Hungarian 
    for (int matrix = 0; matrix < costMatrix.size(); ++matrix)
    {
        std::unique_ptr<AlgoRunner> runner = std::make_unique<AlgoRunner>();
        runner->setAlgorithm(Util::HUNGARIAN);
        runner->setCostMatrix(costMatrix[matrix]);
        runner->setNumTracks(numTracks[matrix]);
        runner->setNumDets(numDets[matrix]);
        runner->run();
        // runner->print_assignment();
        runner->populateAssignmentArray(assignmentArray);

        for (int ii =0; ii < assignmentArray.size(); ++ii)
        {
            std::cout << assignmentArray[ii] << ", ";
        }
        std::cout << std::endl;
    }


    return 0;
}
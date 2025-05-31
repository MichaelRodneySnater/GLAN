#include "SceneParser.hpp"
#include "AlgoRunner.hpp"
#include "Utilties.hpp"

int main(void)
{
    std::vector<std::vector<std::vector<double>>> costMatrix;
    std::unique_ptr<SceneParser> sceneReader = std::make_unique<SceneParser>(costMatrix);
    sceneReader->ReadInCsv();

    // Run the Hungarian
    std::unique_ptr<AlgoRunner> runner = std::make_unique<AlgoRunner>();
    runner->setAlgorithm(Util::HUNGARIAN);
    runner->setCostMatrix(costMatrix[0]);
    runner->run();
    runner->print_assignment();
    // // Read in the scene data from output.csv
    // std::vector<FrameData> frames = sceneRead->readInScenes();

    // // Specify the algorithm, run, generate performance data
    // lapSolverTool->setAlgorithm("hungarian");
    // lapSolverTool->runScenes();
    // lapSolverTool->calculateMetrics();
    // lapSolverTool->generateOuputFile();

    return 0;
}
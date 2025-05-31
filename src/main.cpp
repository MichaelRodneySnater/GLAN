#include "SceneParser.hpp"
#include "Utilties.hpp"

int main(void)
{
    std::vector<std::vector<std::vector<double>>> costMatrix;
    std::unique_ptr<SceneParser> sceneReader = std::make_unique<SceneParser>(costMatrix);
    sceneReader->ReadInCsv();

    std::cout << "Number of Matricies: " << costMatrix.size() << std::endl;

    // // Read in the scene data from output.csv
    // std::vector<FrameData> frames = sceneRead->readInScenes();

    // // Specify the algorithm, run, generate performance data
    // lapSolverTool->setAlgorithm("hungarian");
    // lapSolverTool->runScenes();
    // lapSolverTool->calculateMetrics();
    // lapSolverTool->generateOuputFile();

    return 0;
}
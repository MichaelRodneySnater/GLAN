#include "SceneParser.hpp"
#include "Tracker.hpp"
#include <memory>

int main(void)
{
    std::unique_ptr<SceneParser> sceneReader = std::make_unique<SceneParser>();
    sceneReader->ReadInCsv();

    // // Read in the scene data from output.csv
    // std::vector<FrameData> frames = sceneRead->readInScenes();

    // // Specify the algorithm, run, generate performance data
    // lapSolverTool->setAlgorithm("hungarian");
    // lapSolverTool->runScenes();
    // lapSolverTool->calculateMetrics();
    // lapSolverTool->generateOuputFile();

    return 0;
}
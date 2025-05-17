#include "SceneParser.hpp"

int main(void)
{
    SceneParser sceneRead = new SceneParser();
    AlgoHandler algoRunner = new algoHandler();

    // Read in the scene data from output.csv
    std::vector<FrameData> frames = sceneRead->readInScenes();

    // Specify the algorithm, run, generate performance data
    algoRunner->setAlgorithm("hungarian");
    algoRunner->runScenes();
    algoRunner->calculateMetrics();
    algoRunner->generateOuputFile();

    return 0;
}
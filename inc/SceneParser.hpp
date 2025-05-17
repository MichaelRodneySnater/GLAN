# pragma once
#include <string>
#include "FrameData.hpp"

class SceneParser {
    public:
        SceneParser();
        ~SceneParser();
        void ReadInCsv(void);

    private:

        std::string fileName_;
        std::vector<FrameData> frameData_;
        int numberFrames_;
        const int MAX_FRAMES = 1000;
};
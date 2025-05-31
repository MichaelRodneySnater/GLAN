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
        std::vector<std::vector<double>> costMatrix_;
        int numMatricies_;
        const int MAX_MATRICIES = 1000;
};
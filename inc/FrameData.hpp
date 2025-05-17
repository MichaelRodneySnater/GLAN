#include "TrkUtility.hpp"

class FrameData
{
    public:
        FrameData();
        ~FrameData();

        std::vector<Detection> detList_;
        std::vector<Track> trackList_;
        int frameNumber_;
        std::vector<int> truthAssingment_;
    private:
};
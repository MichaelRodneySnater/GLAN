#pragma once
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <memory>
#include <limits>
#include <iomanip>

namespace Util
{

enum ALGORITHM {
    HUNGARIAN,
    AUCTION,
    JV,
    BINNING
};

}
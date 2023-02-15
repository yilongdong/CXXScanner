#pragma once
#include <vector>
#include "model/CXXInclusion.h"
namespace tudumper::context {
    class CXXAnalysisContext {
    public:
        std::vector<model::CXXInclusion> inclusions;
    };
}
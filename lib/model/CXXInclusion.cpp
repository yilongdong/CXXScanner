#include "model/CXXInclusion.h"


std::string CXXScanner::model::CXXInclusion::dump() const {
    return filename.u8string() + " in " + loc.path.u8string();
}
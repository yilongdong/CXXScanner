#include "model/CXXInclusion.h"


std::string tudumper::model::CXXInclusion::dump() const {
    return filename.filename().u8string() + " in " + loc.filename.filename().u8string();
}
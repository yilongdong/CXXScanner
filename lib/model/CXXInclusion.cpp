#include "model/CXXInclusion.h"


std::string tudumper::model::CXXInclusion::dump() const {
    return filename.u8string() + " in " + loc.filename.u8string();
}
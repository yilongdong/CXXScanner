#pragma once
#include <vector>
#include <set>
#include <regex>
#include "model/CXXTranslationUnit.h"

namespace CXXScanner::context {
    class CXXAnalysisContext {
    public:
        model::CXXTranslationUnit translationUnit;
//        std::vector<model::CXXInclusion> inclusions;
//        std::set<model::CXXClass, model::CXXClass::Less> classes;
//        std::filesystem::path path;

        static bool isSkipHeader(std::filesystem::path const& path);
        static bool isSkipClassName(std::string const& classname);
    };
}
#pragma once
#include <vector>
#include <set>
#include <regex>
#include <filesystem>
#include "model/File.pb.h"

namespace CXXScanner::context {
    class CXXAnalysisContext {
    public:
        std::map<std::string, beacon::model::FileInfo> filesModelMap;
        std::map<std::string const* const, std::map<int, std::vector<beacon::model::FileInfo> > > testType;
        std::string sourceFilePath;
        static bool isSkipHeader(std::filesystem::path const& path);
        static bool isSkipClassName(std::string const& classname);
    };
}
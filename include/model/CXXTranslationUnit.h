#pragma once
#include <set>
#include <fmt/format.h>
#include "model/CXXClass.h"
#include "model/CXXInclusion.h"


namespace CXXScanner::model {
    class CXXTranslationUnit {
    public:
        std::vector<model::CXXInclusion> inclusions;
        std::set<model::CXXClass, model::CXXClass::Less> classes;
        std::filesystem::path path;
        [[nodiscard]] std::string id() const {
            size_t inclusionsHash = 0;
            for (auto const& inclusion : inclusions) {
                inclusionsHash = inclusionsHash ^ inclusion.id();
            }
            std::string prefix_path = path.parent_path().u8string();
            size_t prefixHash = std::hash<std::string>{}(prefix_path);
            std::string filename = path.filename();
            return fmt::format("{}@prefixHash={}@inclusionsHash={}", filename, prefixHash, inclusionsHash);
        };
    };
}

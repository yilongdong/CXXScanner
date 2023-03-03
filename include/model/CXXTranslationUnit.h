#pragma once
#include <set>
#include <fmt/format.h>
#include "model/CXXClass.h"
#include "model/CXXInclusion.h"


namespace CXXScanner::model {
    class CXXTranslationUnit : public model::Element {
    public:
        std::vector<model::CXXInclusion> inclusions;
        std::set<model::CXXClass, model::CXXClass::Less> classes;
        std::filesystem::path path;

    private:
        [[nodiscard]] Element::id_t id() const override {
            size_t inclusions_hash = 0;
            for (auto const& inclusion : inclusions) {
                inclusions_hash = inclusions_hash ^ inclusion.getId();
            }
            std::string prefix_path = path.parent_path().u8string();
            size_t prefix_hash = std::hash<std::string>{}(prefix_path);
            std::string filename = path.filename();
            std::string id_label = fmt::format("{}@prefixHash={}@inclusionsHash={}",
                                               filename, prefix_hash, inclusions_hash);
            return std::hash<std::string>{}(id_label);
        };
    };
}

#pragma once
#include <string>

#include "model/Element.h"

namespace CXXScanner::model {
    class CXXInclusion : public Element {
    public:
        CXXInclusion() = default;
        ~CXXInclusion() override = default;
        [[nodiscard]] std::string dump() const;
        std::filesystem::path filename;
        SourceLocation loc;

    private:
        [[nodiscard]] Element::id_t id() const override {
            return std::hash<std::string>{}(dump());
        }
    };
}

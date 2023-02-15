#pragma once
#include <string>

#include "model/Element.h"

namespace tudumper::model {
    class CXXInclusion : public Element {
    public:
        std::filesystem::path filename;

        [[nodiscard]] std::string dump() const override;
    };
}

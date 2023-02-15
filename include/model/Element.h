#pragma once
#include "model/SourceLoaction.h"

namespace tudumper::model {
    class Element {
    public:
        virtual ~Element() = default;
        [[nodiscard]] virtual std::string dump() const = 0;
        SourceLocation loc;
    };
}

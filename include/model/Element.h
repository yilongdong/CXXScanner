#pragma once
#include "model/SourceLoaction.h"

namespace CXXScanner::model {
    class Element {
    public:
        using id_t = size_t;
        Element() = default;
        virtual ~Element() = default;

        [[nodiscard]] virtual size_t id() const {
            return loc.id();
        }
        SourceLocation loc;
    };
}

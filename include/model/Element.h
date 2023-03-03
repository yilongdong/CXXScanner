#pragma once
#include "model/SourceLoaction.h"
#include <optional>
namespace CXXScanner::model {
    class Element {
    public:
        using id_t = size_t;
        Element() = default;
        virtual ~Element() = default;

        [[nodiscard]] id_t getId() const {
            if (has_stored) return stored_id;
            stored_id = id();
            has_stored = true;
            return stored_id;
        }
    private:
        [[nodiscard]] virtual id_t id() const = 0;
        mutable id_t stored_id{0};
        mutable bool has_stored{false};

    };
}

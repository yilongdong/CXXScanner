#pragma once
#include "model/CXXMethod.h"
#include "model/CXXField.h"
#include "model/CXXBase.h"
#include <vector>

namespace CXXScanner::model {

    class CXXClass : public Element {
    public:
        struct Less {
            bool operator()(CXXClass const& lhs, CXXClass const& rhs) const {
                return lhs.getId() < rhs.getId();
            }
        };

        CXXClass() = default;
        ~CXXClass() override = default;

        SourceLocation loc;
        std::vector<CXXMethod> methods;
        std::vector<CXXField> fields;
        std::vector<CXXBase> bases;
        std::string name;
        bool is_struct{false};

    private:
        [[nodiscard]] Element::id_t id() const override {
            return std::hash<std::string>{}(name);
        }
    };
}


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
                return lhs.loc.dump() < rhs.loc.dump();
            }
        };

        CXXClass() = default;
        ~CXXClass() override = default;
        std::vector<CXXMethod> methods;
        std::vector<CXXField> fields;
        std::vector<CXXBase> bases;
        std::string name;
        bool is_struct{false};
    };
}


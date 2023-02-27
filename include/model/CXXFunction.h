#pragma once
#include "model/Element.h"
#include "model/CXXVariable.h"
#include <string>
#include <vector>

namespace CXXScanner::model {
    class CXXFunction {
    public:
        class CXXParam : public CXXScanner::model::CXXVariable {
        public:
            std::string default_value;
        };

        CXXFunction() = default;
        virtual ~CXXFunction() = default;

        std::string name;
        std::vector<CXXParam> params;
        CXXScanner::model::CXXType return_type;
    };
}

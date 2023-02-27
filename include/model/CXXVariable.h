#pragma once
#include "model/CXXType.h"
namespace CXXScanner::model {
    class CXXVariable {
    public:
        CXXVariable() = default;
        virtual ~CXXVariable() = default;
        CXXType type;
        std::string name;
    };
}

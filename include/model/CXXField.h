#pragma once
#include "model/CXXVariable.h"
#include "model/CXXAccess.h"

namespace CXXScanner::model {
    class CXXField : public CXXVariable {
    public:
        CXXField() = default;
        ~CXXField() override = default;
        CXXAccess access{CXXScanner::model::CXXAccess::AC_UNKNOWN};
    };
}

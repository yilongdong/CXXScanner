#pragma once
#include "model/CXXVariable.h"
#include "model/CXXAccess.h"

namespace CXXScanner::model {
    class CXXField : public CXXVariable {
    public:
        CXXField() = default;
        virtual ~CXXField() = default;
        CXXAccess access{CXXScanner::model::CXXAccess::AC_UNKNOWN};
    };
}

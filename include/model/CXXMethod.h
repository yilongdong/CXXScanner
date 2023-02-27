#pragma once
#include "model/CXXFunction.h"
#include "model/CXXAccess.h"

namespace CXXScanner::model {
    class CXXMethod : public CXXFunction {

    public:
        CXXMethod() = default;
        ~CXXMethod() override = default;
        bool is_deleted{false};
        bool is_default{false};
        bool is_virtual{false};
        bool is_pure{false};
        CXXAccess access{CXXScanner::model::CXXAccess::AC_UNKNOWN};
    };
}
#pragma once
#include "model/CXXType.h"
#include "model/CXXAccess.h"

namespace CXXScanner::model {
    class CXXBase {
    public:
        CXXBase() = default;
        virtual ~CXXBase() = default;
        CXXType type;
        bool is_virtual{false};
        CXXAccess access{CXXScanner::model::CXXAccess::AC_UNKNOWN};
    };
}

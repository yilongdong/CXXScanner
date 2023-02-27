#pragma once
#include <string>
#include "model/Element.h"
namespace CXXScanner::model {
    class CXXType : public Element {
    public:
        CXXType() = default;
        ~CXXType() override = default;
        std::string name;
    };
}

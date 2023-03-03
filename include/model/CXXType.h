#pragma once
#include <string>
#include "model/Element.h"
namespace CXXScanner::model {
    class CXXType : public Element {
    public:
        CXXType() = default;
        ~CXXType() override = default;
        std::string name;
        bool is_buildin{true};

        std::string bare() const {
            return name;
        }
    private:
        [[nodiscard]] Element::id_t id() const override {
            return std::hash<std::string>{}(bare());
        }
    };
}

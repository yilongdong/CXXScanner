#pragma once
#include "model/Element.h"
#include "model/CXXVariable.h"
#include <string>
#include <vector>

namespace CXXScanner::model {


    class CXXFunction : public model::Element {
    public:
        class CXXParam : public CXXScanner::model::CXXVariable {
        public:
            std::string default_value;
        };

        CXXFunction() = default;
        ~CXXFunction() override = default;

        [[nodiscard]] std::string dump() const {
            std::vector<std::string> params_str_vec;
            std::transform(params.begin(), params.end(), std::back_inserter(params_str_vec), [](CXXParam const& param) {
                return fmt::format("{} {}", param.type.name, param.name);
            });
            return fmt::format("{} {}({})", return_type.name, name, fmt::join(params_str_vec, ", "));
        }

        std::string name;
        std::vector<CXXParam> params;
        CXXScanner::model::CXXType return_type;
        SourceLocation declaration_loc;
        SourceLocation definition_loc;

    private:
        [[nodiscard]] Element::id_t id() const override {
            return std::hash<std::string>{}(dump());
        }
    };
}

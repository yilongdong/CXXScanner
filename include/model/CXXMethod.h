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
        bool is_static{false};
        CXXAccess access{CXXScanner::model::CXXAccess::AC_UNKNOWN};

    private:
        [[nodiscard]] Element::id_t id() const override {
            std::vector<std::string> params_str_vec;
            std::transform(params.begin(), params.end(), std::back_inserter(params_str_vec), [](CXXParam const& param) {
                return fmt::format("{} {}", param.type.name, param.name);
            });
            std::string label = fmt::format("{} {}({})", return_type.name, name, fmt::join(params_str_vec, ", "));
            return std::hash<std::string>{}(label);
        }
    };
}
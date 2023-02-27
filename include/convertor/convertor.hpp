#pragma once
#include <functional>


namespace CXXScanner::convertor {
    template <typename from_type, typename to_type>
    class Convertor {
    public:
        to_type convert(from_type const& from) {
            throw std::runtime_error{"未实现的转换"};
        }
    };


}

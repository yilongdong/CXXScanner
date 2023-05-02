#pragma once
#include <string>
#include <vector>
#include <functional>

namespace CXXScanner::callback {

    class CLOCPlugin {
    public:
        struct Result {
            std::string path;
            uint32_t blank = 0;
            uint32_t comment = 0;
            uint32_t code = 0;
            std::string language;
        };

        using Callback = std::function<void(Result const&)>;

        static int scan(std::vector<std::string> const& filesList,const Callback& callback);
    };

}

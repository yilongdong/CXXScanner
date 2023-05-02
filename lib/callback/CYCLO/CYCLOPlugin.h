#pragma once
#include <string>

namespace CXXScanner::callback {

    class CYCLOPlugin {
    public:
        struct Result {
            struct FuncInfo {
                std::string functionName;
                uint32_t cyclomaticComplexity = 0;
                uint32_t lineOfCode = 0;
                uint32_t tokenCount = 0;
                uint32_t parameterCount = 0;
            };
            std::string path;
            std::vector<FuncInfo> functionInfoList;
        };
        using Callback = std::function<void(Result const&)>;
        static int scan(std::vector<std::string> filesList, Callback const& callback);
    };

}

#pragma once
#include <vector>
#include <string>
#include <functional>

namespace CXXScanner::callback {
    class GitPlugin {
    public:
        struct Result {
            struct GitInfo {
                std::string date;
                std::string commitId;
            };
            std::string path;
            std::vector<GitInfo> gitInfoList;
        };
        using Callback = std::function<void(Result const&)>;
        static int scan(std::vector<std::string> const& filesList, Callback const& callback);
    };

}

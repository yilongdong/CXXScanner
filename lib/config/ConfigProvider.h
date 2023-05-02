#pragma once
#include <string>
#include <vector>

namespace CXXScanner::config {

    struct InputConfig {
        std::string build_directory;
        std::vector <std::string> source_file_list;
    };

    struct OutputConfig {
        std::string result_directory;
        bool with_callgraph{false};
    };

    class ConfigProvider {
    public:
        ConfigProvider() = default;
        ~ConfigProvider() = default;
        void loadFromFile(std::string const &filename);

        InputConfig inputConfig;
        OutputConfig outputConfig;
    };
}
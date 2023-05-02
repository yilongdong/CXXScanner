#include "config/ConfigProvider.h"
#include "utility/strutil.h"
#include "utility/log.h"
#include <toml.hpp>

namespace CXXScanner::config {

    void ConfigProvider::loadFromFile(const std::string &filename) {
        toml::basic_value data = toml::parse(filename);

        auto const& input = toml::find(data, "input");
        inputConfig.build_directory = toml::find<std::string>(input, "build_directory");
        inputConfig.source_file_list = toml::find<std::vector<std::string>>(input, "source_file_list");

        auto const& output = toml::find(data, "output");
        outputConfig.result_directory = toml::find_or<std::string>(output, "result_directory", "CXXScanner.dump");

        LOG_INFO("inputConfig.build_directory = {}", inputConfig.build_directory);
        LOG_INFO("inputConfig.source_file_list = {}", fmt::join(inputConfig.source_file_list,", "));
    }
}
#include <gflags/gflags.h>
#include <clang/Tooling/Tooling.h>
#include <clang/Tooling/CompilationDatabase.h>

#include "utility/log.h"
#include "action/FrontendAction.h"

DEFINE_string(conf, "", "配置文件");
DEFINE_bool(verbose, false, "");

int main(int argc, char* argv[]) {
    gflags::ParseCommandLineFlags(&argc, &argv, true);
    CXXScanner::config::ConfigProvider configProvider;
    configProvider.loadFromFile(FLAGS_conf);
    if(FLAGS_verbose) {
        SET_LOGLEVEL("trace");
    }

    LOG_INFO("options (compdb: {}, output_dir: {}, files: {}, verbose: {} )",
             configProvider.inputConfig.build_directory,
             configProvider.outputConfig.result_directory,
             fmt::join(configProvider.inputConfig.source_file_list, ", "),
             FLAGS_verbose);

    std::string errorMsg;
    if (auto db =
            clang::tooling::CompilationDatabase::loadFromDirectory(configProvider.inputConfig.build_directory, errorMsg)) {
        if (configProvider.inputConfig.source_file_list.empty()) {
            configProvider.inputConfig.source_file_list = db->getAllFiles();
        }
        auto const& files = configProvider.inputConfig.source_file_list;
        std::for_each(files.begin(), files.end(), [](std::string const& file) {
            LOG_INFO("file = {}", file);
        });
        clang::tooling::ClangTool tool(*db, files);
        CXXScanner::action::FrontendActionFactory factory(configProvider);
        tool.run(&factory);
    } else {
        LOG_ERROR("Fail to load compile database from {} with error msg {}", configProvider.inputConfig.build_directory, errorMsg);
    }

    return 0;
}

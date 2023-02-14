#include <iostream>
#include <cxxopts.hpp>
#include "utility/log.h"

int main(int argc, char* argv[]) {
    cxxopts::Options options("tu-dumper", "将C++代码转换为json模型的小工具");
    options.add_options()
            ("j,json", "输出json格式", cxxopts::value<bool>()->default_value("true"))
            ("o,output", "输出文件的路径", cxxopts::value<std::string>())
            ("c,command", "编译指令", cxxopts::value<std::string>())
            ("d,directory", "工作目录", cxxopts::value<std::string>())
            ("f,file", "源文件路径", cxxopts::value<std::string>())
            ("v,verbose", "输出详细日志", cxxopts::value<bool>()->default_value("false"));
    auto result = options.parse(argc, argv);
    std::string command = result["command"].as<std::string>();
    std::string directory = result["directory"].as<std::string>();
    std::string file = result["file"].as<std::string>();
    std::string output = result["output"].as<std::string>();
    bool isJsonFormat = result["json"].as<bool>();
    bool isVerbose = result["verbose"].as<bool>();

    LOG_INFO("options \n(command: {},\ndir: {},\nfile: {},\noutput: {},\njson: {},\nverbose: {} )",
             command, directory, file, output, isJsonFormat, isVerbose);
    return 0;
}

#include "context/CXXAnalysisContext.h"

bool CXXScanner::context::CXXAnalysisContext::isSkipHeader(std::filesystem::path const& path) {
    static std::vector<std::regex> regexs = {
        std::regex("^/Library/Developer/CommandLineTools/SDKs/.*"),
        std::regex(R"(^/usr/local/bin/\.\./include/c\+\+/.*)"),
        std::regex("^/usr/local/include/gflags/.*"),
        std::regex("^/usr/local/include/llvm/.*"),
        std::regex("^/usr/local/include/llvm-c/.*"),
        std::regex("^/usr/local/include/clang/.*"),
        std::regex("^/usr/local/include/clang-c/.*"),
        std::regex("^/usr/local/include/gtest/.*"),
        std::regex("^/Users/dongyilong/projects/Clion/TUDumper/build/.*"),
        std::regex("^/opt/homebrew/.*"),
    };
    return std::any_of(regexs.begin(), regexs.end(), [&path](auto const& regex) {
        return std::regex_match(path.u8string(), regex);
    });
}

bool CXXScanner::context::CXXAnalysisContext::isSkipClassName(const std::string &classname) {
    static std::vector<std::regex> regexs = {
            std::regex("^std::.*"),
            std::regex("^google.*"),
    };
    return std::any_of(regexs.begin(), regexs.end(), [&classname](auto const& regex) {
        return std::regex_match(classname, regex);
    });
}

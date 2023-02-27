#pragma once
#include <filesystem>

namespace CXXScanner::model {
    class SourceLocation final {
    public:
        SourceLocation() = default;
        SourceLocation(unsigned int line, unsigned int column, std::string const& path)
            : line{line}, column{column}, path{path} {}
        [[nodiscard]] std::string dump() const {
            return path.u8string() + ":" + std::to_string(line) + ":" + std::to_string(column);
        }
        [[nodiscard]] size_t id() const {
            return std::hash<std::string>{}(dump());
        }
        unsigned int line = -1;
        unsigned int column = -1;
        std::filesystem::path path;
    };
}

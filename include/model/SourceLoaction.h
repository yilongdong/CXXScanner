#pragma once
#include <filesystem>

namespace tudumper::model {
    class SourceLocation {
    public:
        SourceLocation() = default;
        SourceLocation(unsigned int line, unsigned int column, std::string const& filename)
            : line{line}, column{column}, filename{filename} {}

        unsigned int line = -1;
        unsigned int column = -1;
        std::filesystem::path filename;
    };
}

#include "utility/util.h"
std::pair<int, std::string> exec(std::string command, bool with_stderr)
{
    char buffer[1280];
    std::string result;
//    std::system(command.c_str());
    if (with_stderr) {
        command = fmt::format("sh -c '{}' 2>&1", command);
    }

    FILE* pipe = popen(command.c_str(), "r");
    if (!pipe) {
        return {errno, fmt::format("popen failed: {}", strerror(errno))};
    }

    // read till end of process:
    while (!feof(pipe)) {
        // use buffer to read and add to result
        if (fgets(buffer, sizeof(buffer), pipe) != nullptr)
            result += buffer;
    }

    pclose(pipe);
    return {0, result};
}
#include "GitPlugin.h"
#include <fmt/format.h>
#include "utility/util.h"
#include "utility/strutil.h"
#include "utility/log.h"
#include <sstream>

namespace CXXScanner::callback {
    int GitPlugin::scan(const std::vector<std::string> &filesList, const GitPlugin::Callback &callback) {
        for (auto const& filePath : filesList) {
            std::string cmd = fmt::format(R"(git --no-pager log --pretty=format:"%H %ad" --date=format:"%Y-%m-%d %H:%M:%S" {})", filePath);
//            LOG_DEBUG(cmd);
            auto const [ret, output] = exec(cmd, false);
            if(ret != 0 || output.empty()) {
                return -1;
            }
            LOG_DEBUG("[git] output = {}", output);
            Result result;
            result.path = filePath;
            std::stringstream sstream(output);
            std::string commitId, date, time;
            while(sstream >> commitId >> date >> time) {
                Result::GitInfo gitInfo;
                gitInfo.commitId = commitId;
                gitInfo.date = date + " " + time;
                result.gitInfoList.push_back(gitInfo);
            }
            callback(result);
        }

        return 0;
    }
}
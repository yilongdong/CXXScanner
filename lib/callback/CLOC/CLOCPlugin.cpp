#include "CLOCPlugin.h"
#include "utility/util.h"
#include "utility/log.h"
#include "nlohmann/json.hpp"

namespace {
    void getResultFromJson(nlohmann::json const& jsonObj, std::string const& path, CXXScanner::callback::CLOCPlugin::Result& result) {
        auto const& partJsonObj = jsonObj[path];
        result.comment = partJsonObj["comment"];
        result.blank = partJsonObj["blank"];
        result.code = partJsonObj["code"];
        result.language = partJsonObj["language"];
        result.path = path;
    }
}


namespace CXXScanner::callback {
    using namespace nlohmann;

    int CLOCPlugin::scan(std::vector<std::string> const& filesList, const Callback& callback) {
        if(filesList.empty()) {
            return 0;
        }
        std::string cmd = fmt::format("cloc {} --json --by-file", fmt::join(filesList, " "));
        LOG_DEBUG(cmd);
        auto const [ret, output] = exec(cmd, false);
        if(ret != 0) {
            return -1;
        }
        json jsonResult = json::parse(output);
        for(auto const& path: filesList) {
            Result result;
            getResultFromJson(jsonResult, path, result);
            callback(result);
        }

        return 0;
    }
}
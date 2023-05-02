#include "CYCLOPlugin.h"
#include "utility/util.h"
#include "utility/log.h"
#include <fmt/format.h>
#include <sstream>
#include <map>

namespace {
    std::vector<std::string> parseCSVRow(std::string const& str) {
        std::vector<std::string> row;
        bool inQuota = false;
        std::string cell;
        for(char ch : str) {
            if (inQuota) {
                if(ch == '"') {
                    inQuota = false;
                }
                else {
                    cell.push_back(ch);
                }

            }
            else {
                if (ch == '"') {
                    inQuota = true;
                }
                else if(ch == ',') {
                    row.push_back(cell);
                    cell.clear();
                }
                else {
                    cell.push_back(ch);
                }
            }
        }
        if(!cell.empty()) {
            row.push_back(cell);
        }
        return row;
    }
}

namespace CXXScanner::callback {

    int CYCLOPlugin::scan(std::vector<std::string> filesList, const CYCLOPlugin::Callback &callback) {
        if(filesList.empty()) {
            return 0;
        }
        std::string cmd = fmt::format("lizard {} --csv", fmt::join(filesList, " "));
//        LOG_DEBUG(cmd);
        auto const [ret, output] = exec(cmd, false);
        if(ret != 0) {
            return -1;
        }
        std::stringstream sstream(output);
        std::string rowStr;
        std::map<std::string, Result> cycloResultMap;
        while(std::getline(sstream, rowStr)) {
            auto rowValues = parseCSVRow(rowStr);
            std::string path = rowValues.at(6);
            auto& result = cycloResultMap[path];
            result.path = path;
            Result::FuncInfo funcInfo;
            funcInfo.lineOfCode = std::stoul(rowValues.at(0));
            funcInfo.cyclomaticComplexity = std::stoul(rowValues.at(1));
            funcInfo.tokenCount = std::stoul(rowValues.at(2));
            funcInfo.parameterCount = std::stoul(rowValues.at(3));
            funcInfo.functionName = rowValues.at(8);
            result.functionInfoList.push_back(funcInfo);
        }

        for(auto const& [path, result]: cycloResultMap) {
            callback(result);
        }

        return 0;
    }
}
#include "CXXCrossTUContext.h"
#include <google/protobuf/util/json_util.h>
#include <fstream>
#include <ctime>
#include <chrono>
#include <sstream>
#include "utility/log.h"

namespace {
    bool protoToJson(const google::protobuf::Message& message, std::string& json) {
        google::protobuf::util::JsonPrintOptions options;
        options.add_whitespace = true;
        options.always_print_primitive_fields = true;
        options.preserve_proto_field_names = true;
        return MessageToJsonString(message, &json, options).ok();
    }
}

namespace CXXScanner::context {

    bool CXXCrossTUContext::isScannedFile(const std::string &fileId) {
        return scannedFiles.find(fileId) != scannedFiles.end();
    }

    bool CXXCrossTUContext::addFileInfo(beacon::model::FileInfo &&fileInfo) {
        if(!fileInfo.has_id() || isScannedFile(fileInfo.id())) {
            return false;
        }
        LOG_INFO("[cross tu] add new File Model {}", fileInfo.path());
        scannedFiles.insert(fileInfo.id());
        projectDumpFormat.mutable_fileinfolist()->Add(std::move(fileInfo));
        LOG_INFO("[cross tu] file list size = {}", projectDumpFormat.fileinfolist_size());
        return true;
    }

    void CXXCrossTUContext::dump(const std::string &path) {
        time_t rawTime = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
        std::stringstream ss;
        ss << std::put_time(std::localtime(&rawTime), "%c %Z");
        projectDumpFormat.set_id("test");
        projectDumpFormat.set_date(ss.str());
        projectDumpFormat.set_name("test");
        std::string jsonStr;
        if(protoToJson(projectDumpFormat, jsonStr)) {
            std::ofstream fout(path);
            fout << jsonStr;
            fout.close();
        }
    }
}
#pragma once
#include "model/Project.pb.h"
#include <string>
#include <set>

namespace CXXScanner::context {
    class CXXCrossTUContext {
    public:
        bool isScannedFile(std::string const& fileId);
        bool addFileInfo(beacon::model::FileInfo && fileInfo);
        void dump(std::string const& path);
    private:
        std::set<std::string> scannedFiles;
        beacon::model::ProjectDumpFormat projectDumpFormat;
    };
}

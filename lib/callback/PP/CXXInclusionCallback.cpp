#include "utility/log.h"
#include "callback/PP/CXXInclusionCallback.h"

#include <utility>

using namespace CXXScanner::callback;

CXXInclusionCallback::CXXInclusionCallback(clang::Preprocessor &PP) : PP{PP} {
    LOG_DEBUG("construct");
}

CXXInclusionCallback::~CXXInclusionCallback() {
    LOG_DEBUG("destruct");
};

void CXXInclusionCallback::InclusionDirective(clang::SourceLocation HashLoc, const clang::Token &IncludeTok,
                                              llvm::StringRef FileName, bool IsAngled,
                                              clang::CharSourceRange FilenameRange,
                                              llvm::Optional<clang::FileEntryRef> File,
                                              llvm::StringRef SearchPath,
                                              llvm::StringRef RelativePath, const clang::Module *Imported,
                                              clang::SrcMgr::CharacteristicKind FileType) {
    clang::FullSourceLoc fullSourceLoc{HashLoc, PP.getSourceManager()};
    if (fullSourceLoc.isInvalid() || fullSourceLoc.getPresumedLoc().isInvalid()) {
        return;
    }
    beacon::model::CXXInclusion cxxInclusion;
    cxxInclusion.set_path(File ? File->getName().str() : "");
    cxxInclusion.mutable_location()->set_path(fullSourceLoc.getPresumedLoc().getFilename());
    cxxInclusion.mutable_location()->set_line(fullSourceLoc.getPresumedLoc().getLine());
    cxxInclusion.mutable_location()->set_column(fullSourceLoc.getPresumedLoc().getColumn());

    if (filter(cxxInclusion)) {
        return;
    }
    consume(cxxInclusion);
}

void CXXInclusionCallback::setFilterCallback(std::function<bool(const ProductType &)> filterCB) {
    this->filter = std::move(filterCB);
}

void CXXInclusionCallback::setConsumeCallback(std::function<void(const ProductType &)> consumeCB) {
    this->consume = std::move(consumeCB);
}

#include "utility/log.h"
#include "callback/PP/CXXInclusionCallback.h"

using namespace tudumper::callback;

CXXInclusionCallback::CXXInclusionCallback(clang::Preprocessor &PP, ConsumerFunction consume)
    :PP{PP}, consume{std::move(consume)} {
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
    tudumper::model::CXXInclusion inclusionProduct;
    inclusionProduct.filename = File->getName().str();
    inclusionProduct.loc = model::SourceLocation{
            fullSourceLoc.getPresumedLoc().getLine(),
            fullSourceLoc.getPresumedLoc().getColumn(),
            fullSourceLoc.getPresumedLoc().getFilename(),
    };
//    LOG_TRACE("from {}", inclusionProduct.loc.filename.u8string());
//    LOG_TRACE("to {}", inclusionProduct.filename.u8string());
    consume(inclusionProduct);
}



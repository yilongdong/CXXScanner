#include "utility/log.h"
#include "callback/PP/CXXInclusionCallback.h"

using namespace tudumper::callback;

CXXInclusionCallback::CXXInclusionCallback(clang::Preprocessor &PP, ConsumerFunction consume)
    :PP{PP}, consume{consume} {}

CXXInclusionCallback::~CXXInclusionCallback() {}

bool isValidLoc(clang::FullSourceLoc const& fullSourceLoc) {
    if (!fullSourceLoc.hasManager()) {
        LOG_CRITICAL("FullSourceLoc without SrcMgr");
        return false;
    }
    auto presumedLoc = fullSourceLoc.getPresumedLoc();
    return fullSourceLoc.isValid() && presumedLoc.isValid();
}

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
    consume(inclusionProduct);
}



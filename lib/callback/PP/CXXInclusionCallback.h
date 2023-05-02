#pragma once
#include <functional>
#include <clang/Lex/PPCallbacks.h>
#include <clang/Lex/Preprocessor.h>
#include <clang/Basic/SourceManager.h>
#include "model/File.pb.h"

namespace CXXScanner::callback {
    class CXXInclusionCallback : public clang::PPCallbacks {
    public:
        using ProductType = beacon::model::CXXInclusion;
        using ConsumeCallback = std::function<void(ProductType const&)>;
        explicit CXXInclusionCallback(clang::Preprocessor &PP);
        ~CXXInclusionCallback() override;

        void InclusionDirective(clang::SourceLocation HashLoc, const clang::Token &IncludeTok,
                                llvm::StringRef FileName, bool IsAngled,
                                clang::CharSourceRange FilenameRange,
                                llvm::Optional<clang::FileEntryRef> File,
                                llvm::StringRef SearchPath,
                                llvm::StringRef RelativePath, const clang::Module *Imported,
                                clang::SrcMgr::CharacteristicKind FileType) override;
        using FilterCallback = std::function<bool(ProductType const&)>;
        void setFilterCallback(std::function<bool(ProductType const&)> filterCB);
        void setConsumeCallback(std::function<void(ProductType const&)> consumeCB);
    private:
        clang::Preprocessor &PP;
        ConsumeCallback consume;
        FilterCallback filter;
    };
}

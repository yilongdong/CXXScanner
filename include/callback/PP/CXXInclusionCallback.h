#pragma once
#include <functional>
#include <clang/Lex/PPCallbacks.h>
#include <clang/Lex/Preprocessor.h>
#include <clang/Basic/SourceManager.h>
#include "model/CXXInclusion.h"

namespace tudumper::callback {
    class CXXInclusionCallback : public clang::PPCallbacks {
    public:
        using ProductType = typename tudumper::model::CXXInclusion;
        using ConsumerFunction = std::function<void(ProductType const&)>;
        CXXInclusionCallback(clang::Preprocessor &PP, ConsumerFunction consume);
        ~CXXInclusionCallback() override;

        void InclusionDirective(clang::SourceLocation HashLoc, const clang::Token &IncludeTok,
                                llvm::StringRef FileName, bool IsAngled,
                                clang::CharSourceRange FilenameRange,
                                llvm::Optional<clang::FileEntryRef> File,
                                llvm::StringRef SearchPath,
                                llvm::StringRef RelativePath, const clang::Module *Imported,
                                clang::SrcMgr::CharacteristicKind FileType) override;

    private:
        clang::Preprocessor &PP;
        ConsumerFunction consume;
    };
}

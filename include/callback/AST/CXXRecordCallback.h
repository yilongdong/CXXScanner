#pragma once
#include <functional>
#include <clang/AST/Decl.h>
#include <clang/ASTMatchers/ASTMatchFinder.h>
#include <clang/Frontend/CompilerInstance.h>
#include "model/CXXClass.h"

namespace CXXScanner::callback {

    class CXXRecordCallback : public clang::ast_matchers::MatchFinder::MatchCallback {
    public:

        using ConsumerCallback = std::function<void(CXXScanner::model::CXXClass const&)>;
        using FilterCallback = std::function<bool(CXXScanner::model::CXXClass const&)>;
        explicit CXXRecordCallback(clang::CompilerInstance &CI): CI(CI){}
        ~CXXRecordCallback() override = default;
        void run(clang::ast_matchers::MatchFinder::MatchResult const& Result) override;

        CXXRecordCallback& setConsumeCallback(ConsumerCallback callback);
        CXXRecordCallback& setFilterCallback(FilterCallback callback);
    private:
        clang::CompilerInstance &CI;
        ConsumerCallback consume;
        FilterCallback filter;
    };
}

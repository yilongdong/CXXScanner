#pragma once
#include <unordered_set>
#include <clang/AST/Decl.h>
#include <clang/ASTMatchers/ASTMatchFinder.h>
#include <clang/Frontend/CompilerInstance.h>

namespace beacon::callback {
        class CXXCallGraphCallback : public clang::ast_matchers::MatchFinder::MatchCallback {
        public:
            explicit CXXCallGraphCallback(clang::CompilerInstance &CI, beacon::model::TU &TU);
            ~CXXCallGraphCallback() override = default;
            void run(clang::ast_matchers::MatchFinder::MatchResult const& Result) override;

        private:
            clang::CompilerInstance &CI;
        };
    }
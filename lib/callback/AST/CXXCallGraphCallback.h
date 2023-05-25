#pragma once
#include <functional>
#include <clang/AST/Decl.h>
#include <clang/ASTMatchers/ASTMatchFinder.h>
#include <clang/Analysis/CallGraph.h>
#include <clang/Frontend/CompilerInstance.h>
#include "model/File.pb.h"
#include "convertor/ClangDeclToCXXModel.h"


namespace CXXScanner::callback {
    class CXXCallGraphCallback : public clang::ast_matchers::MatchFinder::MatchCallback {
    public:
        using ConsumeCallback = std::function<void(std::string const&, std::unique_ptr<beacon::model::CallGraph>)>;
        explicit CXXCallGraphCallback(clang::CompilerInstance &CI);
        ~CXXCallGraphCallback() override = default;
        void run(clang::ast_matchers::MatchFinder::MatchResult const& Result) override;

        [[nodiscard]] static bool isInStdFunction(std::string const& functionName) ;
        [[nodiscard]] static bool isValidFunctionDecl(clang::Decl const* decl) ;
        CXXCallGraphCallback& setConsumeCallback(ConsumeCallback callback);
    private:
        clang::CompilerInstance &CI;
        ConsumeCallback consume;
        convertor::ClangDeclToCXXModelConvertor convertor;
    };
}
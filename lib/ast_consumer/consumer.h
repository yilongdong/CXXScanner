#include <clang/ASTMatchers/ASTMatchFinder.h>
#include <clang/Frontend/CompilerInstance.h>
#include "callback/AST/CXXRecordCallback.h"
#include "callback/AST/CXXCallGraphCallback.h"
#include "context/CXXAnalysisContext.h"

namespace CXXScanner::ast_consumer {
    class AnalysisConsumer : public clang::ASTConsumer {
    public:
        AnalysisConsumer(clang::CompilerInstance &CI, CXXScanner::context::CXXAnalysisContext& context);
        ~AnalysisConsumer() override;
        bool HandleTopLevelDecl(clang::DeclGroupRef D) override;
        void HandleTranslationUnit(clang::ASTContext &Context) override;

    private:
        clang::ast_matchers::MatchFinder matchFinder;
        CXXScanner::callback::CXXRecordCallback cxxRecordCallback;
        CXXScanner::callback::CXXCallGraphCallback cxxCallGraphCallback;
        CXXScanner::context::CXXAnalysisContext& context;
    };
}

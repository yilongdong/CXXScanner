#include <clang/ASTMatchers/ASTMatchFinder.h>
#include <clang/Frontend/CompilerInstance.h>

namespace tudumper::ast_consumer {
    class AnalysisConsumer : public clang::ASTConsumer {
    public:
        explicit AnalysisConsumer(clang::CompilerInstance &CI);
        ~AnalysisConsumer() override;
        bool HandleTopLevelDecl(clang::DeclGroupRef D) override;
        void HandleTranslationUnit(clang::ASTContext &Context) override;
    private:
        clang::ast_matchers::MatchFinder matchFinder;
    };
}

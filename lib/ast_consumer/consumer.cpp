#include "ast_consumer/consumer.h"

CXXScanner::ast_consumer::AnalysisConsumer::AnalysisConsumer(clang::CompilerInstance &CI, CXXScanner::context::CXXAnalysisContext& context)
    : cxxRecordCallback{CI}, context{context} {
    static clang::ast_matchers::DeclarationMatcher const recordMatcher =
            clang::ast_matchers::cxxRecordDecl(clang::ast_matchers::isClass(), clang::ast_matchers::isDefinition()).bind("class");
    static clang::ast_matchers::DeclarationMatcher const TUMatcher = clang::ast_matchers::translationUnitDecl().bind("TU");

    cxxRecordCallback.setConsumeCallback([this](auto& clsModel) {
        // auto& tuFileModel = this->context.filesModelMap[this->context.sourceFilePath];
        auto& fileModel = this->context.filesModelMap[clsModel.location().path()];
        fileModel.mutable_tuinfo()->mutable_classlist()->Add(std::move(clsModel));
    });

    cxxRecordCallback.setFilterCallback([this](auto const& clsModel)->bool {
        using CXXContext = CXXScanner::context::CXXAnalysisContext;
        auto& fileModel = this->context.filesModelMap[clsModel.location().path()];

        return clsModel.name().empty() || clsModel.location().path().empty() ||
               CXXContext::isSkipClassName(clsModel.name()) ||
               CXXContext::isSkipHeader(clsModel.location().path());
    });

    matchFinder.addMatcher(recordMatcher, &cxxRecordCallback);
}

CXXScanner::ast_consumer::AnalysisConsumer::~AnalysisConsumer() = default;

bool CXXScanner::ast_consumer::AnalysisConsumer::HandleTopLevelDecl(clang::DeclGroupRef D) {
    return ASTConsumer::HandleTopLevelDecl(D);
}

void CXXScanner::ast_consumer::AnalysisConsumer::HandleTranslationUnit(clang::ASTContext &Context) {
    matchFinder.matchAST(Context);
}
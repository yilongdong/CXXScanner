#include "ast_consumer/consumer.h"
#include "utility/log.h"

CXXScanner::ast_consumer::AnalysisConsumer::AnalysisConsumer(clang::CompilerInstance &CI, CXXScanner::context::CXXAnalysisContext& context)
    : cxxRecordCallback{CI}, cxxCallGraphCallback{CI}, context{context} {
    static clang::ast_matchers::DeclarationMatcher const recordMatcher =
            clang::ast_matchers::cxxRecordDecl(clang::ast_matchers::isClass(), clang::ast_matchers::isDefinition()).bind("class");
    static clang::ast_matchers::DeclarationMatcher const TUMatcher = clang::ast_matchers::translationUnitDecl().bind("TU");

    cxxRecordCallback.setConsumeCallback([this](auto& clsModel) {
        // auto& tuFileModel = this->context.filesModelMap[this->context.sourceFilePath];
        auto& fileModel = this->context.filesModelMap[clsModel.location().path()];
        fileModel.mutable_tuinfo()->mutable_classlist()->Add(std::move(clsModel));
    }).setFilterCallback([this](auto const& clsModel)->bool {
        using CXXContext = CXXScanner::context::CXXAnalysisContext;
        auto& fileModel = this->context.filesModelMap[clsModel.location().path()];
        auto hasExist = std::any_of(fileModel.tuinfo().classlist().begin(), fileModel.tuinfo().classlist().end(),
                     [&clsModel](beacon::model::CXXClass const& clsMsg) {
           return  clsMsg.id() == clsModel.id();
        });
        return hasExist || clsModel.name().empty() || clsModel.location().path().empty() ||
               CXXContext::isSkipClassName(clsModel.name()) ||
               CXXContext::isSkipHeader(clsModel.location().path());
    });
    matchFinder.addMatcher(recordMatcher, &cxxRecordCallback);

    cxxCallGraphCallback.setConsumeCallback([this](std::string const& path, std::unique_ptr<beacon::model::CallGraph> callGraphMsg) {
        auto& fileModel = this->context.filesModelMap[path];
        LOG_INFO("[callgraph] path={}, size={}", path, callGraphMsg.get()->relations_size());
        fileModel.mutable_tuinfo()->set_allocated_callgraph(callGraphMsg.release());

    });
    matchFinder.addMatcher(TUMatcher, &cxxCallGraphCallback);
}

CXXScanner::ast_consumer::AnalysisConsumer::~AnalysisConsumer() = default;

bool CXXScanner::ast_consumer::AnalysisConsumer::HandleTopLevelDecl(clang::DeclGroupRef D) {
    return ASTConsumer::HandleTopLevelDecl(D);
}

void CXXScanner::ast_consumer::AnalysisConsumer::HandleTranslationUnit(clang::ASTContext &Context) {
    matchFinder.matchAST(Context);
}
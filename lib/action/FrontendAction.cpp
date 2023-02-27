#include <clang/Frontend/CompilerInstance.h>
#include "utility/log.h"
#include "action/FrontendAction.h"
#include "callback/PP/CXXInclusionCallback.h"
#include "reporter/TUReporter.h"
#include "ast_consumer/consumer.h"

using namespace CXXScanner::action;

std::unique_ptr<clang::ASTConsumer>
FrontendAction::CreateASTConsumer(clang::CompilerInstance &CI, clang::StringRef InFile) {
    clang::Preprocessor &PP = CI.getPreprocessor();
    cxxAnalysisContext.translationUnit.path = std::filesystem::path{InFile.str()};

    LOG_DEBUG("add pp callback");
    // 此处callback::CXXInclusionCallback的生命周期OK
    auto cxxInclusionPPCallback = std::make_unique<callback::CXXInclusionCallback>(PP);
    cxxInclusionPPCallback->setConsumeCallback([this](model::CXXInclusion const& include) {
        this->cxxAnalysisContext.translationUnit.inclusions.push_back(include);
    });
    cxxInclusionPPCallback->setFilterCallback([this](model::CXXInclusion const& include)->bool {
        return CXXScanner::context::CXXAnalysisContext::isSkipHeader(include.loc.path);
    });
    PP.addPPCallbacks(std::move(cxxInclusionPPCallback));

    // 运行cloc
    return std::make_unique<CXXScanner::ast_consumer::AnalysisConsumer>(CI, cxxAnalysisContext);
}

void FrontendAction::EndSourceFileAction() {
//    std::string srcFilename = cxxAnalysisContext.path.filename();
//    std::string savePath = "/Users/dongyilong/projects/Clion/TUDumper/result/inclusion/" + srcFilename + ".dot";

//    CXXScanner::reporter::Reporter reporter;
//    auto const& inclusions = cxxAnalysisContext.translationUnit.inclusions;
//    reporter.loadInclusions(inclusions.begin(), inclusions.end())
//            .setName("inclusion")
//            .savePath(savePath)
//            .doReport();

    CXXScanner::reporter::TUReporter tuReporter;
    tuReporter.loadTranslationUnit(cxxAnalysisContext.translationUnit).report();
    LOG_INFO("report finish");
    // graphql
}

std::unique_ptr<clang::FrontendAction> FrontendActionFactory::create() {
    LOG_DEBUG("make frontend action");
    return std::make_unique<::CXXScanner::action::FrontendAction>();
}

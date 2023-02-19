#include <clang/Frontend/CompilerInstance.h>
#include "utility/log.h"
#include "action/FrontendAction.h"
#include "callback/PP/CXXInclusionCallback.h"
#include "reporter/reporter.h"
#include "consumer.h"

using namespace tudumper::action;

std::unique_ptr<clang::ASTConsumer>
FrontendAction::CreateASTConsumer(clang::CompilerInstance &CI, clang::StringRef InFile) {
    clang::Preprocessor &PP = CI.getPreprocessor();
    cxxAnalysisContext.path = std::filesystem::path{InFile.str()};

    LOG_DEBUG("add pp callback");
    // 此处callback::CXXInclusionCallback的生命周期OK
    PP.addPPCallbacks(std::make_unique<callback::CXXInclusionCallback>(PP, [this](model::CXXInclusion const& include) {
        this->cxxAnalysisContext.inclusions.push_back(include);
    }));

    // 运行cloc
    return std::make_unique<tudumper::ast_consumer::AnalysisConsumer>(CI);
}

void FrontendAction::EndSourceFileAction() {
    std::string srcFilename = cxxAnalysisContext.path.filename();
    std::string savePath = "/Users/dongyilong/projects/Clion/TUDumper/result/inclusion/" + srcFilename + ".dot";

    tudumper::reporter::Reporter reporter;
    auto const& inclusions = cxxAnalysisContext.inclusions;
    reporter.loadInclusions(inclusions.begin(), inclusions.end())
            .setName("inclusion")
            .savePath(savePath)
            .doReport();
}

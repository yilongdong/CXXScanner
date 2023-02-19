#include <clang/Frontend/CompilerInstance.h>
#include "utility/log.h"
#include "action/FrontendAction.h"
#include "callback/PP/CXXInclusionCallback.h"

using namespace tudumper::action;

std::unique_ptr<clang::ASTConsumer>
FrontendAction::CreateASTConsumer(clang::CompilerInstance &CI, clang::StringRef InFile) {
    clang::Preprocessor &PP = CI.getPreprocessor();
    // this的生命周期大于callback::CXXInclusionCallback
    PP.addPPCallbacks(std::make_unique<callback::CXXInclusionCallback>(PP, [this](model::CXXInclusion const& include) {
        this->cxxAnalysisContext.inclusions.push_back(include);
    }));

    return std::unique_ptr<clang::ASTConsumer>{};//std::make_unique<AnalysisConsumer>(CI, CXXContext.TU);
}

void FrontendAction::EndSourceFileAction() {
    for(auto const& include : cxxAnalysisContext.inclusions) {
        LOG_INFO("include: {}", include.dump());
    }
}

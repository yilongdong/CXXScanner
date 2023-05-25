#include <clang/Frontend/CompilerInstance.h>
#include "utility/log.h"
#include "action/FrontendAction.h"
#include "callback/PP/CXXInclusionCallback.h"
#include "callback/CLOC/CLOCPlugin.h"
#include "callback/CYCLO/CYCLOPlugin.h"
#include "callback/Git/GitPlugin.h"
#include "ast_consumer/consumer.h"

using namespace CXXScanner::action;

std::unique_ptr<clang::ASTConsumer>
FrontendAction::CreateASTConsumer(clang::CompilerInstance &CI, clang::StringRef InFile) {
    std::filesystem::path inFilePath(InFile.str());
    LOG_DEBUG("in File = {}", inFilePath.string());
    cxxAnalysisContext.sourceFilePath = inFilePath.string();

    LOG_DEBUG("add pp callback");
    // 此处callback::CXXInclusionCallback的生命周期OK
    clang::Preprocessor &PP = CI.getPreprocessor();
    auto cxxInclusionPPCallback = std::make_unique<callback::CXXInclusionCallback>(PP);
    cxxInclusionPPCallback->setConsumeCallback([this](beacon::model::CXXInclusion const& include) {
        auto& filesModelMap = this->cxxAnalysisContext.filesModelMap;
        auto& fileModel = filesModelMap[include.location().path()];
        std::filesystem::path filePath{include.location().path()};
        if (!fileModel.has_id()) {
            fileModel.set_id(filePath.string());
            fileModel.set_path(filePath.string());
            fileModel.set_directory(filePath.parent_path().string());
            fileModel.set_filename(filePath.filename().string());
        }
        bool hasExist = std::any_of(fileModel.inclusions().begin(), fileModel.inclusions().end(), [&include](auto const& inclusion) {
            return inclusion.path() == include.path();
        });
        if (!hasExist) {
            auto pInclusion = fileModel.add_inclusions();
            pInclusion->set_path(include.path());
            pInclusion->set_column(include.location().column());
            pInclusion->set_line(include.location().line());
        }
    });
    cxxInclusionPPCallback->setFilterCallback([](beacon::model::CXXInclusion const& include)->bool {
        bool shouldSkip = CXXScanner::context::CXXAnalysisContext::isSkipHeader(include.location().path());
        return shouldSkip;
    });
    PP.addPPCallbacks(std::move(cxxInclusionPPCallback));

    // 运行cloc
    return std::make_unique<CXXScanner::ast_consumer::AnalysisConsumer>(CI, cxxAnalysisContext);
}

void FrontendAction::EndSourceFileAction() {
    bool withCloc = true, withGit = true, withLizard = true;
    std::vector<std::string> filesList;
    for (auto const& [path, _] : this->cxxAnalysisContext.filesModelMap) {
        if(path.empty()) continue;
        filesList.push_back(path);
    }
    if(withCloc) {
        CLOCScan(filesList);
    }

    if (withLizard) {
        CYCLOScan(filesList);
    }

    if(withGit) {
        GitScan(filesList);
    }
    m_endSourceFileActionCallback(cxxAnalysisContext);
}

void FrontendAction::CLOCScan(const std::vector<std::string> &filesList) {
    callback::CLOCPlugin::scan(filesList, [this](callback::CLOCPlugin::Result const& result) {
        auto& fileModel = this->cxxAnalysisContext.filesModelMap[result.path];
        fileModel.mutable_clocinfo()->set_language(result.language);
        fileModel.mutable_clocinfo()->set_blank(result.blank);
        fileModel.mutable_clocinfo()->set_code(result.code);
        fileModel.mutable_clocinfo()->set_comment(result.comment);

//        LOG_INFO("[cloc] file:{} code:{} comment:{} blank:{} language:{}",
//                 result.path, result.code, result.comment, result.blank, result.language);
    });
}

void FrontendAction::CYCLOScan(const std::vector<std::string> &filesList) {
    callback::CYCLOPlugin::scan(filesList, [this](callback::CYCLOPlugin::Result const &result) {
        auto &fileModel = this->cxxAnalysisContext.filesModelMap[result.path];
        for (auto const &funcCycloInfo: result.functionInfoList) {
            auto cycloInfo = fileModel.add_cycloinfolist();
//            auto cycloInfo = fileModel.mutable_cycloinfolist()->Add();
            cycloInfo->set_functionname(funcCycloInfo.functionName);
            cycloInfo->set_cyclomaticcomplexity(funcCycloInfo.cyclomaticComplexity);
            cycloInfo->set_lineofcode(funcCycloInfo.lineOfCode);
            cycloInfo->set_parametercount(funcCycloInfo.parameterCount);
            cycloInfo->set_tokencount(funcCycloInfo.tokenCount);
//            LOG_DEBUG("[cyclo] func = {} loc = {} cyclo = {}", funcCycloInfo.functionName, funcCycloInfo.lineOfCode,
//                      funcCycloInfo.cyclomaticComplexity);
        }
//        LOG_DEBUG("[cyclo] path = {} func count = {}", result.path, result.functionInfoList.size());
    });
}

void FrontendAction::GitScan(const std::vector<std::string> &filesList) {
    callback::GitPlugin::scan(filesList, [this](callback::GitPlugin::Result const &result) {
        auto &fileModel = this->cxxAnalysisContext.filesModelMap[result.path];
        for (auto const &gitInfoModel: result.gitInfoList) {
            auto gitInfo = fileModel.add_gitinfo();
            gitInfo->set_date(gitInfoModel.date);
            gitInfo->set_commitid(gitInfoModel.commitId);
//            LOG_DEBUG("[git] commit id = {} date = {}", gitInfoModel.commitId, gitInfoModel.date);
        }
    });
}

FrontendAction::~FrontendAction() = default;

std::unique_ptr<clang::FrontendAction> FrontendActionFactory::create() {
    LOG_DEBUG("make frontend action");
    return std::make_unique<CXXScanner::action::FrontendAction>([this](context::CXXAnalysisContext& context) {
        for (auto& [path, fileInfo] : context.filesModelMap) {
//            LOG_INFO("tu context fileModelMap size = {}", context.filesModelMap.size());
            this->cxxCrossTuContext.addFileInfo(std::move(fileInfo));
        }
        LOG_DEBUG("add new tu");
    });
}

FrontendActionFactory::FrontendActionFactory(const CXXScanner::config::ConfigProvider &configProvider)
    : configProvider(configProvider) {}

FrontendActionFactory::~FrontendActionFactory() {
    LOG_INFO("save dump file");
    cxxCrossTuContext.dump(configProvider.outputConfig.result_directory + "/test.dump.json");
    LOG_INFO("finish dump file");
}
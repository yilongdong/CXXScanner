#pragma once
#include <functional>
#include <utility>
#include <clang/Frontend/FrontendActions.h>
#include <clang/Tooling/Tooling.h>
#include "context/CXXAnalysisContext.h"
#include "context/CXXCrossTUContext.h"
#include "config/ConfigProvider.h"

namespace CXXScanner::action {
    class FrontendAction : public clang::ASTFrontendAction {
    public:
        using EndSourceFileActionCallback = std::function<void(context::CXXAnalysisContext&)> ;
        explicit FrontendAction(EndSourceFileActionCallback callback)
            : m_endSourceFileActionCallback(std::move(callback)) {}
        ~FrontendAction() override;
    protected:
        std::unique_ptr<clang::ASTConsumer> CreateASTConsumer(clang::CompilerInstance &CI, clang::StringRef InFile) override;
        void EndSourceFileAction() override;

    private:
        void CLOCScan(std::vector<std::string> const& filesList);
        void CYCLOScan(std::vector<std::string> const& filesList);
        void GitScan(std::vector<std::string> const& filesList);
    private:
        context::CXXAnalysisContext cxxAnalysisContext;
        EndSourceFileActionCallback m_endSourceFileActionCallback;
    };

    class FrontendActionFactory : public clang::tooling::FrontendActionFactory {
    public:
        ~FrontendActionFactory() override;
        explicit FrontendActionFactory(config::ConfigProvider const& configProvider);
        std::unique_ptr<clang::FrontendAction> create() override;
    private:
        config::ConfigProvider const& configProvider;
        context::CXXCrossTUContext cxxCrossTuContext;

    };
}
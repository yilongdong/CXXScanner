#pragma once
#include <functional>
#include <clang/AST/Decl.h>
#include <clang/ASTMatchers/ASTMatchFinder.h>
#include <clang/Frontend/CompilerInstance.h>
#include "model/File.pb.h"
#include "convertor/ClangDeclToCXXModel.h"

namespace CXXScanner::callback {

    class CXXRecordCallback : public clang::ast_matchers::MatchFinder::MatchCallback {
    public:

        using ConsumerCallback = std::function<void(beacon::model::CXXClass&)>;
        using FilterCallback = std::function<bool(beacon::model::CXXClass const&)>;
        explicit CXXRecordCallback(clang::CompilerInstance &CI): CI(CI), convertor(CI){}
        ~CXXRecordCallback() override = default;
        void run(clang::ast_matchers::MatchFinder::MatchResult const& Result) override;

        CXXRecordCallback& setConsumeCallback(ConsumerCallback callback);
        CXXRecordCallback& setFilterCallback(FilterCallback callback);


    private:
        void visitBase(beacon::model::CXXClass& ClassMsg, clang::CXXRecordDecl const& ClassDecl);
        void visitMethod(beacon::model::CXXClass& ClassMsg, clang::CXXRecordDecl const& ClassDecl);
        void visitField(beacon::model::CXXClass& ClassMsg, clang::CXXRecordDecl const& ClassDecl);
        void fillClassRelations(beacon::model::CXXClass& ClassMsg);
    private:
        clang::CompilerInstance &CI;
        ConsumerCallback consume;
        FilterCallback filter;
        convertor::ClangDeclToCXXModelConvertor convertor;
    };
}

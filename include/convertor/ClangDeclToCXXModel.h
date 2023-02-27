#pragma once

#include <memory>
#include <clang/AST/Decl.h>
#include <clang/AST/DeclCXX.h>
#include "model/CXXClass.h"
#include <clang/Frontend/CompilerInstance.h>

namespace CXXScanner::convertor {
    class ClangDeclToCXXModelConvertor {
    public:
        explicit ClangDeclToCXXModelConvertor(clang::CompilerInstance& CI):CI{CI} {}
        [[nodiscard]] static CXXScanner::model::CXXAccess getAccess(clang::AccessSpecifier const& accessSpecifier) ;
        bool getBase(clang::CXXBaseSpecifier const& baseSpecifier, CXXScanner::model::CXXBase &) const;
        bool getField(clang::FieldDecl const& fieldDecl, CXXScanner::model::CXXField &) const;
        bool getQualType(clang::QualType const& qualType, CXXScanner::model::CXXType &) const;
        bool getLocation(const clang::SourceLocation &srcLoc, CXXScanner::model::SourceLocation&) const;
        bool getFunction(clang::FunctionDecl const& funcDecl, CXXScanner::model::CXXFunction &) const;
        bool getMethod(clang::CXXMethodDecl const& methodDecl, CXXScanner::model::CXXMethod &) const;
        bool getParam(clang::ParmVarDecl const& paramVarDecl, CXXScanner::model::CXXFunction::CXXParam &) const;
    private:
        clang::CompilerInstance& CI;
    };
}
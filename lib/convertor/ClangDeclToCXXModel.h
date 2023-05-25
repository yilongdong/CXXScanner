#pragma once

#include <memory>
#include <clang/AST/Decl.h>
#include <clang/AST/DeclCXX.h>
#include "model/Code.pb.h"
#include <clang/Frontend/CompilerInstance.h>

namespace CXXScanner::convertor {
    class TypeVisitor {
    public:
        TypeVisitor(clang::CompilerInstance &CI) : CI(CI) {}
        virtual ~TypeVisitor() = default;
        void TraverseType(clang::QualType const& qualType);

        void onPointerType(clang::PointerType const& ptrType);
        void onRefType(clang::ReferenceType const& refType);
        void onArrayType(clang::ArrayType const& arrayType);
        void onFunctionType(clang::FunctionType const& functionType);
        void onRecordType(clang::RecordType const& recordType);
        void onTemplateSpecializationType(clang::TemplateSpecializationType const& templateSpecializationType);
        void onDependentNameType(clang::DependentNameType const& dependentNameType);
        void onElaboratedType(clang::ElaboratedType const& elaboratedType);
        void onDependentTemplateSpecializationType(clang::DependentTemplateSpecializationType const& dependentTemplateSpecializationType);
        std::string getCanonicalUnqualifiedTypeName(clang::QualType const& type);
        void addAssociatedType(clang::QualType const& type);
        bool isUserDefinedType(clang::QualType const& type) const;
        bool isPointerOrArrayOrReferenceOrFunctionType(clang::QualType const& type);
        std::vector<std::string> associatedTypes;
    private:
        clang::CompilerInstance &CI;
    };

    class ClangDeclToCXXModelConvertor {
    public:
        explicit ClangDeclToCXXModelConvertor(clang::CompilerInstance& CI):CI{CI} {}
        std::string getNamespaceString(clang::CXXRecordDecl const& cxxRecordDecl) const;
        [[nodiscard]] std::unique_ptr<beacon::model::CXXAccess> toAccessMsg(clang::AccessSpecifier const& accessSpecifier) const;
        [[nodiscard]] std::unique_ptr<beacon::model::CXXBase> toBaseMsg(clang::CXXBaseSpecifier const& baseSpecifier, bool isVirtual = false) const;
        [[nodiscard]] std::unique_ptr<beacon::model::CXXField> toFieldMsg(clang::FieldDecl const& fieldDecl) const;
        [[nodiscard]] std::unique_ptr<beacon::model::CXXType> toTypeMsg(clang::QualType const& qualType) const;
        [[nodiscard]] std::unique_ptr<beacon::model::Location> toLocationMsg(clang::SourceLocation const& srcLoc) const;
        [[nodiscard]] std::unique_ptr<beacon::model::CXXFunction> toFunctionMsg(clang::CXXMethodDecl const& methodDecl) const;
        [[nodiscard]] std::unique_ptr<beacon::model::CXXFunction> toFunctionMsg(clang::FunctionDecl const& functionDecl) const;
        [[nodiscard]] std::unique_ptr<beacon::model::CXXParam> toParamMsg(clang::ParmVarDecl const& paramVarDecl) const;
        [[nodiscard]] std::unique_ptr<beacon::model::CXXClassRelation> toClassRelationMsg(std::string const& from, std::string const& to, beacon::model::CXXClassRelation::RelationKind kind) const;
        [[nodiscard]] std::vector<beacon::model::CXXClassRelation> toClassRelationsMsgFromCXXType(std::string const& from, beacon::model::CXXType const& type, beacon::model::CXXClassRelation::RelationKind kind) const;
        [[nodiscard]] std::unique_ptr<beacon::model::CXXFunctionRelation> toFunctionRelationMsg(std::unique_ptr<beacon::model::CXXFunction> from, std::unique_ptr<beacon::model::CXXFunction> to, beacon::model::CXXFunctionRelation::RelationKind kind) const;
    private:
        [[nodiscard]] std::string accessToString(clang::AccessSpecifier const& accessSpecifier) const;
        [[nodiscard]] std::string accessToSymbol(clang::AccessSpecifier const& accessSpecifier) const;
        [[nodiscard]] std::string fullnameOfFunctionMsg(beacon::model::CXXFunction const& functionMsg) const;

    private:
        clang::CompilerInstance& CI;
    };
}
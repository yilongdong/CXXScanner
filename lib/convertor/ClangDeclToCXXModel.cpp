#include "convertor/ClangDeclToCXXModel.h"


CXXScanner::model::CXXAccess
CXXScanner::convertor::ClangDeclToCXXModelConvertor::getAccess(const clang::AccessSpecifier &accessSpecifier) {
    switch (accessSpecifier) {
        case clang::AccessSpecifier::AS_public:
            return CXXScanner::model::CXXAccess::AC_PUBLIC;
        case clang::AccessSpecifier::AS_protected:
            return CXXScanner::model::CXXAccess::AC_PROTECTED;
        case clang::AccessSpecifier::AS_private:
            return CXXScanner::model::CXXAccess::AC_PRIVATE;
        default:
            return CXXScanner::model::CXXAccess::AC_UNKNOWN;
    }
}

bool CXXScanner::convertor::ClangDeclToCXXModelConvertor::getBase(const clang::CXXBaseSpecifier &baseSpecifier,
                                                                CXXScanner::model::CXXBase & baseModel) const {
    baseModel.access = getAccess(baseSpecifier.getAccessSpecifier());
    getQualType(baseSpecifier.getType(), baseModel.type);
    baseModel.is_virtual = baseSpecifier.isVirtual();
    return true;
}

bool CXXScanner::convertor::ClangDeclToCXXModelConvertor::getField(const clang::FieldDecl &fieldDecl,
                                                                 CXXScanner::model::CXXField & fieldModel) const {
    fieldModel.access = getAccess(fieldDecl.getAccess());
    fieldModel.name = fieldDecl.getQualifiedNameAsString();
    getQualType(fieldDecl.getType(), fieldModel.type);
    return true;
}

bool CXXScanner::convertor::ClangDeclToCXXModelConvertor::getMethod(const clang::CXXMethodDecl &methodDecl,
                                                                  CXXScanner::model::CXXMethod & methodModel) const {
    methodModel.access = getAccess(methodDecl.getAccess());
    methodModel.name = methodDecl.getQualifiedNameAsString();
    getQualType(methodDecl.getReturnType(), methodModel.return_type);
    for (auto const* paramDecl : methodDecl.parameters()) {
        if(paramDecl) {
            CXXScanner::model::CXXFunction::CXXParam param;
            getParam(*paramDecl, param);
            methodModel.params.push_back(param);
        }
    }
    methodModel.is_virtual = methodDecl.isVirtual();
    methodModel.is_deleted = methodDecl.isDeleted();
    methodModel.is_pure = methodDecl.isPure();
    methodModel.is_default = methodDecl.isDeleted();
    methodModel.is_static = methodDecl.isStatic();
    return true;
}

bool CXXScanner::convertor::ClangDeclToCXXModelConvertor::getQualType(const clang::QualType &qualType,
                                                                    CXXScanner::model::CXXType & qualTypeModel) const {
    if (auto tagDecl = qualType->getAsTagDecl()) {
        qualTypeModel.name = tagDecl->getQualifiedNameAsString();
        getLocation(tagDecl->getLocation(), qualTypeModel.loc);
        return true;
    }
    qualTypeModel.name = qualType.getAsString();
    return false;
}

bool CXXScanner::convertor::ClangDeclToCXXModelConvertor::getLocation(const clang::SourceLocation &srcLoc,
                                                                    CXXScanner::model::SourceLocation& srcLocModel) const {
    srcLocModel.line = CI.getSourceManager().getSpellingLineNumber(srcLoc);
    srcLocModel.column = CI.getSourceManager().getSpellingColumnNumber(srcLoc);
    srcLocModel.path = std::filesystem::path(CI.getSourceManager().getFilename(srcLoc).str());
    return true;
}

bool CXXScanner::convertor::ClangDeclToCXXModelConvertor::getFunction(const clang::FunctionDecl &funcDecl,
                                                                    CXXScanner::model::CXXFunction & funcModel) const {
    funcModel.name = funcDecl.getQualifiedNameAsString();
    getQualType(funcDecl.getReturnType(), funcModel.return_type);
    for (auto const* paramDecl : funcDecl.parameters()) {
        if(paramDecl) {
            CXXScanner::model::CXXFunction::CXXParam param;
            getParam(*paramDecl, param);
            funcModel.params.push_back(param);
        }
    }
    return true;
}

bool CXXScanner::convertor::ClangDeclToCXXModelConvertor::getParam(const clang::ParmVarDecl &paramDecl,
                                                                 CXXScanner::model::CXXFunction::CXXParam & paramModel) const {
    paramModel.name = paramDecl.getQualifiedNameAsString();
    getQualType(paramDecl.getType(), paramModel.type);
    return true;
}

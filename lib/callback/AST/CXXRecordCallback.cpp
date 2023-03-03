#include "callback/AST/CXXRecordCallback.h"
#include "convertor/ClangDeclToCXXModel.h"
#include "utility/log.h"

namespace {
    class Generator {
        CXXScanner::convertor::ClangDeclToCXXModelConvertor convertor;
    public:
        explicit Generator(clang::CompilerInstance& CI): convertor(CI) {};
        bool generate(clang::CXXRecordDecl const& recordDecl, CXXScanner::model::CXXClass & cxxClassModel);
    };
}


void CXXScanner::callback::CXXRecordCallback::run(const clang::ast_matchers::MatchFinder::MatchResult &Result) {
    if(auto const*pRecordDecl = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("class")) {
        // convertor to model
        CXXScanner::model::CXXClass cxxClassModel;
        Generator generator(CI);
        bool success = generator.generate(*pRecordDecl, cxxClassModel);

        // filter model
        if(!success || filter(cxxClassModel)) {
            return;
        }

        // consume model
        consume(cxxClassModel);
    }
}

CXXScanner::callback::CXXRecordCallback &CXXScanner::callback::CXXRecordCallback::setConsumeCallback(
        CXXScanner::callback::CXXRecordCallback::ConsumerCallback callback) {
    consume = std::move(callback);
    return *this;
}

CXXScanner::callback::CXXRecordCallback &CXXScanner::callback::CXXRecordCallback::setFilterCallback(
        CXXScanner::callback::CXXRecordCallback::FilterCallback callback) {
    filter = std::move(callback);
    return *this;
}

bool Generator::generate(const clang::CXXRecordDecl &recordDecl, CXXScanner::model::CXXClass &cxxClassModel) {
    if (!recordDecl.isStruct() && !recordDecl.isClass()) {
        return false;
    }
    cxxClassModel.name = recordDecl.getQualifiedNameAsString();
    cxxClassModel.is_struct = recordDecl.isStruct();
    convertor.getLocation(recordDecl.getLocation(), cxxClassModel.loc);

    for (auto const * methodDecl : recordDecl.methods()) {
        if (methodDecl) {
            CXXScanner::model::CXXMethod methodModel;
            convertor.getMethod(*methodDecl, methodModel);
            cxxClassModel.methods.push_back(methodModel);
        }
    }

    for (auto const * fieldDecl : recordDecl.fields()) {
        if (fieldDecl) {
            CXXScanner::model::CXXField fieldModel;
            convertor.getField(*fieldDecl, fieldModel);
            cxxClassModel.fields.push_back(fieldModel);
        }
    }

    for (auto const& baseSpecifier : recordDecl.bases()) {
        CXXScanner::model::CXXBase baseModel;
        convertor.getBase(baseSpecifier, baseModel);
        baseModel.is_virtual = false;
        cxxClassModel.bases.push_back(baseModel);
    }

    for (auto const& baseSpecifier : recordDecl.vbases()) {
        CXXScanner::model::CXXBase baseModel;
        convertor.getBase(baseSpecifier, baseModel);
        baseModel.is_virtual = true;
        cxxClassModel.bases.push_back(baseModel);
    }
    return true;
}
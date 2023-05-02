#include "convertor/ClangDeclToCXXModel.h"
#include "utility/log.h"
namespace {

}
namespace CXXScanner::convertor {

    std::unique_ptr<beacon::model::CXXBase>
    ClangDeclToCXXModelConvertor::toBaseMsg(const clang::CXXBaseSpecifier &baseSpecifier, bool isVirtual) const {
        auto baseMsg = std::make_unique<beacon::model::CXXBase>();
        baseMsg->set_is_virtual(isVirtual);
        baseMsg->set_allocated_access(toAccessMsg(baseSpecifier.getAccessSpecifier()).release());
        baseMsg->set_allocated_type(toTypeMsg(baseSpecifier.getType()).release());
        return baseMsg;
    }

    std::string ClangDeclToCXXModelConvertor::getNamespaceString(
            const clang::CXXRecordDecl &cxxRecordDecl) const {
        std::string namespaceStr = cxxRecordDecl.getQualifiedNameAsString(); // 带命名空间的完全限定名
        size_t classNameLen = cxxRecordDecl.getNameAsString().length(); // 类名
        return namespaceStr.length() > classNameLen ?
               namespaceStr.substr(0, namespaceStr.length() - classNameLen - 2) : "";
    }

    std::string ClangDeclToCXXModelConvertor::accessToString(
            clang::AccessSpecifier const &accessSpecifier) const {
        switch (accessSpecifier) {
            case clang::AccessSpecifier::AS_private:
                return "private";
            case clang::AccessSpecifier::AS_protected:
                return "protected";
            case clang::AccessSpecifier::AS_public:
                return "public";
            case clang::AS_none:
                return "none";
        }
        return "unknown";
    }

    std::string ClangDeclToCXXModelConvertor::accessToSymbol(
            clang::AccessSpecifier const &accessSpecifier) const {
        switch (accessSpecifier) {
            case clang::AccessSpecifier::AS_private:
                return "-";
            case clang::AccessSpecifier::AS_protected:
                return "#";
            case clang::AccessSpecifier::AS_public:
                return "+";
            case clang::AS_none:
                return "";
        }
        return "";
    }

    std::unique_ptr<beacon::model::CXXAccess>
    ClangDeclToCXXModelConvertor::toAccessMsg(clang::AccessSpecifier const &accessSpecifier) const {
        auto accessMsg = std::make_unique<beacon::model::CXXAccess>();
        accessMsg->set_name(accessToString(accessSpecifier));
        accessMsg->set_symbol(accessToSymbol(accessSpecifier));
        return accessMsg;
    }

    std::unique_ptr<beacon::model::CXXField>
    ClangDeclToCXXModelConvertor::toFieldMsg(const clang::FieldDecl &fieldDecl) const {
        auto fieldMsg = std::make_unique<beacon::model::CXXField>();
        fieldMsg->set_name(fieldDecl.getQualifiedNameAsString());
        fieldMsg->set_allocated_access(toAccessMsg(fieldDecl.getAccess()).release());
        fieldMsg->set_allocated_type(toTypeMsg(fieldDecl.getType()).release());
        return fieldMsg;
    }

    std::unique_ptr<beacon::model::CXXType>
    ClangDeclToCXXModelConvertor::toTypeMsg(const clang::QualType &qualType) const {
        auto typeMsg = std::make_unique<beacon::model::CXXType>();
//        typeMsg->set_id(qualType.getCanonicalType().getAsString());
        typeMsg->set_name(qualType.getCanonicalType().getAsString());
        if (!qualType->isBuiltinType()) {
            TypeVisitor typeVisitor(CI);
            typeVisitor.TraverseType(qualType);
            typeMsg->mutable_associatedtypes()->Add(typeVisitor.associatedTypes.begin(), typeVisitor.associatedTypes.end());
        }
        return typeMsg;
    }

    std::unique_ptr<beacon::model::Location>
    ClangDeclToCXXModelConvertor::toLocationMsg(const clang::SourceLocation &srcLoc) const {
        auto locationMsg = std::make_unique<beacon::model::Location>();
        locationMsg->set_path(CI.getSourceManager().getFilename(srcLoc).str());
        locationMsg->set_column(CI.getSourceManager().getSpellingColumnNumber(srcLoc));
        locationMsg->set_line(CI.getSourceManager().getSpellingLineNumber(srcLoc));
        return locationMsg;
    }
    std::string ClangDeclToCXXModelConvertor::fullnameOfFunctionMsg(beacon::model::CXXFunction const& functionMsg) const {
        std::vector<std::string> params;
        std::transform(functionMsg.params().begin(),functionMsg.params().end(), std::back_inserter(params),
                       [](beacon::model::CXXParam const& param) {
                           return param.name();
                       });
        std::string fullname = fmt::format("{} {}({})",
                                           functionMsg.returntype().name(),
                                           functionMsg.name(),
                                           fmt::join(params, ", "));
        return fullname;
    }
    std::unique_ptr<beacon::model::CXXFunction>
    ClangDeclToCXXModelConvertor::toFunctionMsg(const clang::CXXMethodDecl &methodDecl) const {
        auto methodMsg = std::make_unique<beacon::model::CXXFunction>();
        methodMsg->set_name(methodDecl.getNameAsString());
        methodMsg->set_allocated_access(toAccessMsg(methodDecl.getAccess()).release());
        methodMsg->set_allocated_returntype(toTypeMsg(methodDecl.getReturnType()).release());
        if (auto const classDecl = methodDecl.getParent()) {
            methodMsg->set_classname(classDecl->getQualifiedNameAsString());
        }
        for (auto const &paramDecl: methodDecl.parameters()) {
            if (!paramDecl) continue;
            methodMsg->mutable_params()->AddAllocated(toParamMsg(*paramDecl).release());
        }
        methodMsg->set_isstatic(methodDecl.isStatic());
        methodMsg->set_isvirtual(methodDecl.isVirtual());
        methodMsg->set_ispurevirtual(methodDecl.isPure());

        methodMsg->set_fullname(fullnameOfFunctionMsg(*methodMsg));
        // TODO: 赋值更多函数属性
        return methodMsg;
    }

    std::unique_ptr<beacon::model::CXXFunction>
    ClangDeclToCXXModelConvertor::toFunctionMsg(const clang::FunctionDecl &functionDecl) const {
        auto funcMsg = std::make_unique<beacon::model::CXXFunction>();
        funcMsg->set_name(functionDecl.getNameAsString());
        funcMsg->set_allocated_returntype(toTypeMsg(functionDecl.getReturnType()).release());
        for (auto const &paramDecl: functionDecl.parameters()) {
            if (!paramDecl) continue;
            funcMsg->mutable_params()->AddAllocated(toParamMsg(*paramDecl).release());
        }
        funcMsg->set_isstatic(functionDecl.isStatic());

        funcMsg->set_fullname(fullnameOfFunctionMsg(*funcMsg));
        // TODO: 赋值更多函数属性
        return funcMsg;
    }

    std::unique_ptr<beacon::model::CXXParam>
    ClangDeclToCXXModelConvertor::toParamMsg(const clang::ParmVarDecl &paramVarDecl) const {
        auto paramMsg = std::make_unique<beacon::model::CXXParam>();
        paramMsg->set_name(paramVarDecl.getNameAsString());
        paramMsg->set_allocated_type(toTypeMsg(paramVarDecl.getType()).release());
        // TODO: 默认参数
        return paramMsg;
    }

    std::unique_ptr<beacon::model::CXXClassRelation>
    ClangDeclToCXXModelConvertor::toClassRelationMsg(std::string const& from, std::string const& to, beacon::model::CXXClassRelation::RelationKind kind) const {
        auto relation = std::make_unique<beacon::model::CXXClassRelation>();
        relation->set_kind(kind);
        relation->set_from(from);
        relation->set_to(to);
        return relation;
    }

    std::vector<beacon::model::CXXClassRelation>
    ClangDeclToCXXModelConvertor::toClassRelationsMsgFromCXXType(const std::string &from,
                                                                 const beacon::model::CXXType &type,
                                                                 beacon::model::CXXClassRelation::RelationKind kind) const {
        std::vector<beacon::model::CXXClassRelation> relations;
        for(auto const& associatedType : type.associatedtypes()) {
            auto relation = toClassRelationMsg(from, associatedType, kind);
            if(relation) {
                relations.push_back(*relation.release());
            }
        }
        return relations;
    }

    void TypeVisitor::TraverseType(clang::QualType const& qualType) {
        clang::Type const * type = qualType.getTypePtr();
        if(!type) return;
        if (auto ptrType = llvm::dyn_cast<clang::PointerType>(type)) {
            onPointerType(*ptrType);
        } else if (auto refType = llvm::dyn_cast<clang::ReferenceType>(type)) {
            onRefType(*refType);
        } else if (auto arrayType = llvm::dyn_cast<clang::ArrayType>(type)) {
            onArrayType(*arrayType);
        } else if (auto functionType = llvm::dyn_cast<clang::FunctionType>(type)) {
            // 遍历函数类型及函数参数返回值
            onFunctionType(*functionType);
        } else if (auto recordType = llvm::dyn_cast<clang::RecordType>(type)) {
            // 遍历结构体或类的内部成员类型
            onRecordType(*recordType);
        } else if (auto templateType = llvm::dyn_cast<clang::TemplateSpecializationType>(type)) {
            // 遍历模板类型的内部类型
            onTemplateSpecializationType(*templateType);
        } else if (auto dependentType = llvm::dyn_cast<clang::DependentNameType>(type)) {
            // 遍历依赖于模板参数的类型的内部类型
            onDependentNameType(*dependentType);
        } else if (auto elaboratedType = llvm::dyn_cast<clang::ElaboratedType>(type)) {
            // 遍历限定符类型的内部类型
            onElaboratedType(*elaboratedType);
        } else if (auto dependentTemplateSpecializationType =
                llvm::dyn_cast<clang::DependentTemplateSpecializationType>(type)) {
            // 遍历依赖于模板参数的模板类型的内部类型
            onDependentTemplateSpecializationType(*dependentTemplateSpecializationType);
        } else {
            // 处理其他类型
        }
        addAssociatedType(qualType);
    }

    void TypeVisitor::onPointerType(clang::PointerType const& ptrType) {
//        LOG_DEBUG("pointer {}", ptrType.getPointeeType().getAsString());
//        addAssociatedType(ptrType.getPointeeType())
        TraverseType(ptrType.getPointeeType());
        addAssociatedType(ptrType.getPointeeType());
    }
    void TypeVisitor::onRefType(clang::ReferenceType const& refType) {
//        LOG_DEBUG("ref {}", refType.getPointeeType().getAsString());
        TraverseType(refType.getPointeeType());
        addAssociatedType(refType.getPointeeType());
    }
    void TypeVisitor::onArrayType(clang::ArrayType const& arrayType) {
//        LOG_DEBUG("array {}", arrayType.getElementType().getAsString());
        TraverseType(arrayType.getElementType());
        addAssociatedType(arrayType.getElementType());
    }
    void TypeVisitor::onFunctionType(clang::FunctionType const& functionType) {
//        LOG_DEBUG("return type {}", functionType.getReturnType().getAsString());
        TraverseType(functionType.getReturnType());
        if(auto const functionProtoType = functionType.getAs<clang::FunctionProtoType>()) {
            for (auto paramType : functionProtoType->getParamTypes()) {
//                LOG_DEBUG("param type {}", paramType.getAsString());
                TraverseType(paramType);
                addAssociatedType(paramType);
            }
        }
        addAssociatedType(functionType.getReturnType());
    }
    void TypeVisitor::onRecordType(clang::RecordType const& recordType) {
        auto recordDecl = recordType.getDecl();
        addAssociatedType(recordType.getCanonicalTypeInternal());
//        LOG_DEBUG("record {}", recordDecl->getQualifiedNameAsString());
        // 暂时不需要间接关联的类型
        // for (auto field : recordDecl->fields()) {
        //     LOG_DEBUG("field type {}", field->getType().getAsString());
        //     TraverseType(field->getType().getTypePtr());
        // }
    }
    void TypeVisitor::onTemplateSpecializationType(clang::TemplateSpecializationType const& templateSpecializationType) {
        const auto * templateDecl = templateSpecializationType.getTemplateName().getAsTemplateDecl();
        assert(templateDecl != nullptr && "Template declaration not found");
//        LOG_DEBUG("TemplateSpecializationType {}", templateDecl->getQualifiedNameAsString());
        for (auto arg : templateSpecializationType.template_arguments()) {
            if (arg.getKind() == clang::TemplateArgument::Type) {
//                LOG_DEBUG("TemplateSpecializationType args {}", arg.getAsType().getAsString());
                TraverseType(arg.getAsType());
                addAssociatedType(arg.getAsType());
            }
        }
        addAssociatedType((templateSpecializationType.getCanonicalTypeInternal()));
    }
    void TypeVisitor::onDependentNameType(clang::DependentNameType const& dependentNameType) {
//        LOG_DEBUG("dependentType {}", dependentNameType.getCanonicalTypeInternal().getAsString());
//        TraverseType(dependentNameType.getCanonicalTypeInternal());
//        addAssociatedType(dependentNameType.getCanonicalTypeInternal());
    }
    void TypeVisitor::onElaboratedType(clang::ElaboratedType const& elaboratedType) {
//        LOG_DEBUG("elaboratedType {}", elaboratedType.getNamedType().getAsString());
        TraverseType(elaboratedType.getNamedType());
        addAssociatedType(elaboratedType.getNamedType());
    }
    void TypeVisitor::onDependentTemplateSpecializationType(clang::DependentTemplateSpecializationType const& dependentTemplateSpecializationType) {
        uint32_t numArgs = dependentTemplateSpecializationType.getNumArgs();
        for(uint32_t i = 0; i < numArgs; ++i) {
            auto arg = dependentTemplateSpecializationType.getArg(i);
            if (arg.getKind() == clang::TemplateArgument::Type) {
//                LOG_DEBUG("dependentTemplateSpecializationType {}", arg.getAsType().getAsString());
                TraverseType(arg.getAsType());

                addAssociatedType((arg.getAsType()));
            }
            // 其他模板类型暂时没有处理
        }
        addAssociatedType(dependentTemplateSpecializationType.getCanonicalTypeInternal());
    }

    std::string TypeVisitor::getCanonicalUnqualifiedTypeName(clang::QualType const& type) {

        return type.getDesugaredType(CI.getASTContext()).getAsString();
    }
    void TypeVisitor::addAssociatedType(clang::QualType const& type) {
        bool isPointerOrArrayOrReferenceOrFunction = isPointerOrArrayOrReferenceOrFunctionType(type);
        bool isUserDefined = isUserDefinedType(type);
        clang::QualType mutableType = type;
        auto name = getCanonicalUnqualifiedTypeName(type);
        bool hasExist = std::any_of(associatedTypes.begin(), associatedTypes.end(), [&name](auto const& typenName) {
            return typenName == name;
        });

        if(isUserDefined && !hasExist && !isPointerOrArrayOrReferenceOrFunction) {
            if(name == "const std::string *") {
                LOG_ERROR("test error");
            }
            associatedTypes.push_back(name);
        }
    }
    bool TypeVisitor::isUserDefinedType(clang::QualType const& type) const {
        bool isBuiltinType = type->isBuiltinType();
        std::string typeName = type.getCanonicalType().getUnqualifiedType().getAsString();
        bool isStdType = typeName.find("std::") == 0 || typeName.find("class std::") == 0;
        return !isBuiltinType && !isStdType;
    }

    bool TypeVisitor::isPointerOrArrayOrReferenceOrFunctionType(clang::QualType const& type) {
        return
            llvm::dyn_cast<clang::PointerType>(type) ||
            llvm::dyn_cast<clang::ReferenceType>(type) ||
            llvm::dyn_cast<clang::ArrayType>(type) ||
            llvm::dyn_cast<clang::FunctionType>(type);
    }

}
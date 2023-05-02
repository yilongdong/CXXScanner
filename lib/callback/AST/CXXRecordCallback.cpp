#include "callback/AST/CXXRecordCallback.h"
#include "model/Code.pb.h"
#include "utility/log.h"

void CXXScanner::callback::CXXRecordCallback::run(const clang::ast_matchers::MatchFinder::MatchResult &Result) {
    const auto *ClassDecl = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("class");
    if (!ClassDecl || !ClassDecl->getDefinition() || ClassDecl->isLambda()) {
        return;
    }
    beacon::model::CXXClass ClassMsg;
    std::string id = ClassDecl->isClass() ? "class " : "struct ";
    id += ClassDecl->getQualifiedNameAsString();
    ClassMsg.set_id(id);
    ClassMsg.set_name(ClassDecl->getNameAsString());
    ClassMsg.set_isstruct(!ClassDecl->isClass());
    ClassMsg.set_allocated_location(convertor.toLocationMsg(ClassDecl->getLocation()).release());
    ClassMsg.set_namespace_(convertor.getNamespaceString(*ClassDecl));
    ClassMsg.set_allocated_location(convertor.toLocationMsg(ClassDecl->getLocation()).release());
    // TODO: 添加枚举，常亮，别名等

    visitBase(ClassMsg, *ClassDecl);
    visitMethod(ClassMsg, *ClassDecl);
    visitField(ClassMsg, *ClassDecl);

    if (filter(ClassMsg)) {
        return;
    };

    fillClassRelations(ClassMsg);
    consume(ClassMsg);
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


void CXXScanner::callback::CXXRecordCallback::visitBase(beacon::model::CXXClass &ClassMsg, clang::CXXRecordDecl const& ClassDecl) {
    for (const auto &Base: ClassDecl.bases()) {
        ClassMsg.mutable_bases()->AddAllocated(convertor.toBaseMsg(Base, false).release());
    }
    for (const auto &Base: ClassDecl.vbases()) {
        ClassMsg.mutable_bases()->AddAllocated(convertor.toBaseMsg(Base, true).release());
    }
}

void CXXScanner::callback::CXXRecordCallback::visitMethod(beacon::model::CXXClass &ClassMsg,
                                                          const clang::CXXRecordDecl &ClassDecl) {
    for(const auto &Method: ClassDecl.methods()) {
        if(!Method) continue;
        if(Method->isImplicit()) continue;
        ClassMsg.mutable_methods()->AddAllocated(convertor.toFunctionMsg(*Method).release());
    }
}

void CXXScanner::callback::CXXRecordCallback::visitField(beacon::model::CXXClass &ClassMsg,
                                                         const clang::CXXRecordDecl &ClassDecl) {
    for(const auto &Field: ClassDecl.fields()) {
        if(!Field) continue;
        ClassMsg.mutable_fields()->AddAllocated(convertor.toFieldMsg(*Field).release());
    }
}

void uniqueRelations(std::vector<beacon::model::CXXClassRelation>& relations) {
    std::sort(relations.begin(), relations.end(), [](beacon::model::CXXClassRelation const& lhs, beacon::model::CXXClassRelation const& rhs) {
        if(lhs.kind() == rhs.kind()) {
            return lhs.from() + lhs.to() < rhs.from() + rhs.to();
        }
        else {
            return lhs.kind() < rhs.kind();
        }
    });
    relations.erase(std::unique(relations.begin(), relations.end(), [](beacon::model::CXXClassRelation const& lhs, beacon::model::CXXClassRelation const& rhs) {
        return lhs.kind() == rhs.kind() &&
               lhs.from() == rhs.from() &&
               lhs.to() == rhs.to();
    }), relations.end());
}

void CXXScanner::callback::CXXRecordCallback::fillClassRelations(beacon::model::CXXClass &ClassMsg) {
    // 继承关系
    std::vector<beacon::model::CXXClassRelation> relations;
    for(auto const& BaseMsg : ClassMsg.bases()) {
        auto kind = beacon::model::CXXClassRelation_RelationKind_REL_BASE;
        auto rels = convertor.toClassRelationsMsgFromCXXType(ClassMsg.id(), BaseMsg.type(), kind);
        std::copy(rels.begin(), rels.end(), std::back_inserter(relations));
    }

    // 组合关系
    for(auto const& FieldMsg : ClassMsg.fields()) {
        auto kind = beacon::model::CXXClassRelation_RelationKind_REL_COMP;
        auto rels = convertor.toClassRelationsMsgFromCXXType(ClassMsg.id(), FieldMsg.type(), kind);
        std::copy(rels.begin(), rels.end(), std::back_inserter(relations));
    }
    
    // 关联关系
    for(auto const& MethodMsg : ClassMsg.methods()) {
        auto kind = beacon::model::CXXClassRelation_RelationKind_REL_ASSOC;
        auto rels = convertor.toClassRelationsMsgFromCXXType(ClassMsg.id(), MethodMsg.returntype(), kind);
        std::copy(rels.begin(), rels.end(), std::back_inserter(relations));
        for(auto const& ParamMsg : MethodMsg.params()) {
            auto rels2 = convertor.toClassRelationsMsgFromCXXType(ClassMsg.id(), ParamMsg.type(), kind);
            std::copy(rels2.begin(), rels2.end(), std::back_inserter(relations));
        }
    }

    // 去重
    uniqueRelations(relations);
    ClassMsg.mutable_relations()->Add(relations.begin(), relations.end());
}

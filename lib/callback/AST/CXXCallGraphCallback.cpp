#include "CXXCallGraphCallback.h"
#include <clang/Analysis/CallGraph.h>
#include <utility>

namespace CXXScanner::callback {
    CXXCallGraphCallback::CXXCallGraphCallback(clang::CompilerInstance &CI) : CI(CI), convertor(CI) {}

    void CXXCallGraphCallback::run(clang::ast_matchers::MatchFinder::MatchResult const &Result) {
        clang::CallGraph callGraph;
        callGraph.addToCallGraph(CI.getASTContext().getTranslationUnitDecl());
//        callGraph.dump();
        auto callGraphMsg = std::make_unique<beacon::model::CallGraph>();
        for (auto const &[_, callerNode]: callGraph) {
            if (!callerNode) continue;
            clang::Decl const *callerDecl = callerNode->getDecl();
            if (!isValidFunctionDecl(callerDecl)) {
                continue;
            }
            auto const callerFuncMsg = convertor.toFunctionMsg(*callerDecl->getAsFunction());
            if (!callerFuncMsg || isInStdFunction(callerFuncMsg->name())) {
                continue;
            }
            for (auto const &callRecord: callerNode->callees()) {
                clang::CallGraphNode const *calleeNode = callRecord.Callee;
                if(!calleeNode) continue;
                clang::Decl const *calleeDecl = calleeNode->getDecl();
                if (!isValidFunctionDecl(calleeDecl)) {
                    continue;
                }
                auto calleeFuncMsg = convertor.toFunctionMsg(*calleeDecl->getAsFunction());
                if (!calleeFuncMsg || isInStdFunction(calleeFuncMsg->name())) {
                    continue;
                }
                auto callerFuncMsgClone = std::make_unique<beacon::model::CXXFunction>();
                callerFuncMsgClone->CopyFrom(*callerFuncMsg);
                auto relation = convertor.toFunctionRelationMsg(std::move(callerFuncMsgClone), std::move(calleeFuncMsg),
                                                beacon::model::CXXFunctionRelation_RelationKind_REL_CALL);
                callGraphMsg->mutable_relations()->AddAllocated(relation.release());
            }
        }
        auto const mainFileID = CI.getSourceManager().getMainFileID();
        auto const* fileEntry = CI.getSourceManager().getFileEntryForID(mainFileID);
        if(fileEntry) {
            consume(fileEntry->getName().str(), std::move(callGraphMsg));
        }
    }

    CXXCallGraphCallback &CXXCallGraphCallback::setConsumeCallback(CXXCallGraphCallback::ConsumeCallback callback) {
        consume = std::move(callback);
        return *this;
    }

    bool CXXCallGraphCallback::isInStdFunction(std::string const &functionName) {
        return functionName.rfind("std", 0) == 0;
    }

    bool CXXCallGraphCallback::isValidFunctionDecl(clang::Decl const *decl) {
        return decl && !decl->isImplicit() && decl->getAsFunction();
    }
}
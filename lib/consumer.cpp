#include "consumer.h"

tudumper::ast_consumer::AnalysisConsumer::AnalysisConsumer(clang::CompilerInstance &CI) {

}

tudumper::ast_consumer::AnalysisConsumer::~AnalysisConsumer() {

}

bool tudumper::ast_consumer::AnalysisConsumer::HandleTopLevelDecl(clang::DeclGroupRef D) {
    return ASTConsumer::HandleTopLevelDecl(D);
}

void tudumper::ast_consumer::AnalysisConsumer::HandleTranslationUnit(clang::ASTContext &Context) {
    ASTConsumer::HandleTranslationUnit(Context);
}

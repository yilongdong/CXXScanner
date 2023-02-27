add_library(ast_consumer consumer.cpp)
add_library(CXXScanner::ast_consumer ALIAS ast_consumer)
target_link_libraries(ast_consumer PUBLIC ${ClangLLVMLibs})
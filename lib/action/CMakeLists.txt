add_library(action FrontendAction.cpp)
add_library(CXXScanner::action ALIAS action)
target_link_libraries(action PUBLIC ${ClangLLVMLibs} CXXScanner::reporter)
add_library(callback)
add_library(CXXScanner::callback ALIAS callback)
target_link_libraries(callback PUBLIC ${ClangLLVMLibs} CXXScanner::utility)

add_subdirectory(PP)
add_subdirectory(AST)
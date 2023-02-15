add_library(action FrontendAction.cpp)
add_library(tudumper::action ALIAS action)
target_link_libraries(action PUBLIC ${ClangLLVMLibs})
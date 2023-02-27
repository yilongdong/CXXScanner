set(ClangLLVMLibs clangAST clangBasic clangFrontend clangSerialization clangTooling)

add_subdirectory(utility)
add_subdirectory(model)
add_subdirectory(context)
add_subdirectory(dao)
add_subdirectory(reporter)
add_subdirectory(action)
add_subdirectory(callback)
add_subdirectory(ast_consumer)
add_subdirectory(convertor)


add_library(TUDumperLib foo.cpp)
target_link_libraries(TUDumperLib
        PUBLIC
        CXXScanner::utility
        CXXScanner::model
        CXXScanner::context
        CXXScanner::callback
        CXXScanner::action
        CXXScanner::ast_consumer
        CXXScanner::convertor
        CXXScanner::dao
        CXXScanner::reporter)


set(ClangLLVMLibs clangAST clangBasic clangFrontend clangSerialization clangTooling)


find_package(Protobuf CONFIG REQUIRED)
include_directories(
        ${CMAKE_CURRENT_BINARY_DIR}
        ${Protobuf_INCLUDE_DIRS}
        ${CMAKE_CURRENT_SOURCE_DIR}
)
add_subdirectory(utility)
add_subdirectory(model)
add_subdirectory(config)
add_subdirectory(context)
add_subdirectory(action)
add_subdirectory(callback)
add_subdirectory(ast_consumer)
add_subdirectory(convertor)

add_library(CXXScannerLib foo.cpp)
target_link_libraries(CXXScannerLib
        PUBLIC
        CXXScanner::utility
        CXXScanner::model
        CXXScanner::context
        CXXScanner::callback
        CXXScanner::action
        CXXScanner::ast_consumer
        CXXScanner::convertor
        CXXScanner::config
        ${ClangLLVMLibs})
target_include_directories(CXXScannerLib
        PUBLIC ${CMAKE_CURRENT_BINARY_DIR}
        PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
        )


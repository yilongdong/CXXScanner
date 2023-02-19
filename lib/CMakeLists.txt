set(ClangLLVMLibs clangAST clangBasic clangFrontend clangSerialization clangTooling)

add_subdirectory(utility)
add_subdirectory(model)
add_subdirectory(context)
add_subdirectory(action)
add_subdirectory(callback)


add_library(TUDumperLib foo.cpp consumer.cpp)
target_link_libraries(TUDumperLib
        PUBLIC
        tudumper::utility
        tudumper::model
        tudumper::context
        tudumper::callback
        tudumper::action)


cmake_minimum_required(VERSION 3.23)
# 把系统编译器给整坏了，可能是因为没有编译compile-rt之类的，所以这里指定使用brew安装的clang。
set(CMAKE_C_COMPILER /opt/homebrew/opt/llvm/bin/clang)
set(CMAKE_CXX_COMPILER /opt/homebrew/opt/llvm/bin/clang++)
#set(CMAKE_C_COMPILER /usr/local/bin/clang)
#set(CMAKE_CXX_COMPILER /usr/local/bin/clang++)
project(CXXScanner)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug CACHE
            STRING "Build type (default Debug):" FORCE)
endif()
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
# 检测内存泄露与未定义行为
if (CMAKE_BUILD_TYPE MATCHES "^[Dd]ebug")
    message(STATUS "add memcheck")
    link_directories(/opt/homebrew/lib)
    # 内存泄露与未定义行为检查
    set(DEBUG_CONFIG "-fsanitize=undefined,address,leak -fno-omit-frame-pointer -fvisibility=hidden -fvisibility-inlines-hidden")
    # 内存泄露与CPU和堆栈分析(需要gperftools)
    # string(APPEND DEBUG_CONFIG " -lprofiler -ltcmalloc")
    set(CMAKE_CXX_FLAGS ${DEBUG_CONFIG})
    set(CMAKE_C_FLAGS ${DEBUG_CONFIG})
    set(CMAKE_Ｌ_FLAGS ${DEBUG_CONFIG})
endif()

## 生成库文档
include(DocForLibrary)
include(DocForCLITool)

## 引入LibTooling
include(AddLibTooling)

add_subdirectory(lib)
add_subdirectory(standalone)
#add_subdirectory(tests)

## 安装
install(TARGETS CXXScanner DESTINATION bin)
install(TARGETS CXXScannerLib LIBRARY DESTINATION lib)
install(DIRECTORY ${PROJECT_SOURCE_DIR}/lib/ DESTINATION include)
install(FILES CXXScanner-conf.toml DESTINATION etc)
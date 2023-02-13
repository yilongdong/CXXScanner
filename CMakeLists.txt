cmake_minimum_required(VERSION 3.23)
# 乱整编译器给整坏了，为了配合IDE，使用brew安装的clang。
set(CMAKE_C_COMPILER /opt/homebrew/Cellar/llvm/15.0.7_1/bin/clang)
set(CMAKE_CXX_COMPILER /opt/homebrew/Cellar/llvm/15.0.7_1/bin/clang++)

project(tudumper)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

include_directories(include)

add_subdirectory(lib)
add_subdirectory(standalone)
add_subdirectory(tests)

## 生成库文档
include(DocForLibrary)
include(DocForCLITool)
## 安装
install(TARGETS tudumper DESTINATION bin)
install(TARGETS TUDumperLib LIBRARY DESTINATION lib)
install(DIRECTORY ${PROJECT_SOURCE_DIR}/include/ DESTINATION include)
install(FILES tudumper-conf.toml DESTINATION etc)
cmake_minimum_required(VERSION 3.23)

set(CMAKE_C_COMPILER /opt/homebrew/Cellar/llvm/15.0.7_1/bin/clang)
set(CMAKE_CXX_COMPILER /opt/homebrew/Cellar/llvm/15.0.7_1/bin/clang++)

project(tudumper)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include_directories(include)

add_subdirectory(lib)
add_subdirectory(standalone)
add_subdirectory(tests)

## 安装
install(TARGETS tudumper DESTINATION bin)
install(TARGETS TUDumperLib LIBRARY DESTINATION lib)
install(DIRECTORY ${PROJECT_SOURCE_DIR}/include/ DESTINATION include)
install(FILES tudumper-conf.toml DESTINATION etc)
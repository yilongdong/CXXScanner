cmake_minimum_required(VERSION 3.23)
project(tudumper)

set(CMAKE_CXX_STANDARD 17)
include_directories(include)

add_library(TUDumperLib src/foo.cpp include/foo.h)
find_package(spdlog CONFIG REQUIRED)
target_link_libraries(TUDumperLib PUBLIC spdlog::spdlog)

add_executable(tudumper standalone/main.cpp)
target_link_libraries(tudumper PRIVATE TUDumperLib)

## 安装
install(TARGETS tudumper DESTINATION bin)
install(TARGETS TUDumperLib LIBRARY DESTINATION lib)
install(DIRECTORY ${PROJECT_SOURCE_DIR}/include/ DESTINATION include)
install(FILES tudumper-conf.toml DESTINATION etc)
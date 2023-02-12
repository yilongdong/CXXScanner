cmake_minimum_required(VERSION 3.23)
project(tudumper)

set(CMAKE_CXX_STANDARD 17)

find_package(spdlog CONFIG REQUIRED)

include_directories(include)

add_library(TUDumperLib src/foo.cpp include/foo.h)
target_link_libraries(TUDumperLib PUBLIC spdlog::spdlog)

add_executable(tudumper standalone/main.cpp)
target_link_libraries(tudumper PRIVATE TUDumperLib)

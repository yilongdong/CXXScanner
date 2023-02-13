add_library(TUDumperLib foo.cpp)
find_package(spdlog CONFIG REQUIRED)
target_link_libraries(TUDumperLib PUBLIC spdlog::spdlog)
add_executable(tudumper main.cpp)
find_package(gflags REQUIRED)
target_link_libraries(tudumper PRIVATE TUDumperLib gflags::gflags)
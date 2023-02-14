add_subdirectory(utility)

add_library(TUDumperLib foo.cpp)
target_link_libraries(TUDumperLib PUBLIC utility::log)


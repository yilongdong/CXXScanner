find_package(spdlog REQUIRED)
add_library(utility log.cpp strutil.cpp)
add_library(CXXScanner::utility ALIAS utility)
target_link_libraries(utility PUBLIC spdlog::spdlog)

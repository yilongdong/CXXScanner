find_package(spdlog REQUIRED)
add_library(log log.cpp)
add_library(utility::log ALIAS log)
target_link_libraries(log PUBLIC spdlog::spdlog)
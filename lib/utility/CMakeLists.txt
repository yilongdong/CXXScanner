find_package(spdlog REQUIRED)
find_package(fmt CONFIG REQUIRED)
add_library(utility log.cpp log.h strutil.cpp strutil.h singleton.h util.h util.cpp)
add_library(CXXScanner::utility ALIAS utility)
target_link_libraries(utility
        PUBLIC spdlog::spdlog
        PRIVATE fmt::fmt)

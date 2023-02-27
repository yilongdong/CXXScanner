add_library(reporter reporter.cpp TUReporter.cpp)
find_package(fmt REQUIRED)
add_library(CXXScanner::reporter ALIAS reporter)
target_link_libraries(reporter
        PUBLIC
        fmt::fmt
        CXXScanner::dao)
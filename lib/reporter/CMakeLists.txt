add_library(reporter reporter.cpp)
find_package(fmt REQUIRED)
add_library(tudumper::reporter ALIAS reporter)
target_link_libraries(reporter PRIVATE fmt::fmt)
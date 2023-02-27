find_package(fmt REQUIRED)
add_library(model CXXInclusion.cpp)
add_library(CXXScanner::model ALIAS model)
target_link_libraries(model PUBLIC fmt::fmt)
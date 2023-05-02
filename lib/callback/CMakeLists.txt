add_library(callback "" Git/GitPlugin.cpp Git/GitPlugin.h)
add_library(CXXScanner::callback ALIAS callback)

add_subdirectory(PP)
add_subdirectory(AST)
add_subdirectory(CLOC)
add_subdirectory(CYCLO)

target_link_libraries(callback PUBLIC CXXScanner::utility CXXScanner::model)
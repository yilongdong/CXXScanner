add_library(context CXXAnalysisContext.cpp)
add_library(CXXScanner::context ALIAS context)
target_link_libraries(context PUBLIC CXXScanner::model)
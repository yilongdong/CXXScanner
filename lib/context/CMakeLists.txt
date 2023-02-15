add_library(context CXXAnalysisContext.cpp)
add_library(tudumper::context ALIAS context)
target_link_libraries(context PUBLIC tudumper::model)
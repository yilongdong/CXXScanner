add_library(context CXXAnalysisContext.cpp CXXAnalysisContext.h CXXCrossTUContext.cpp CXXCrossTUContext.h)
add_library(CXXScanner::context ALIAS context)
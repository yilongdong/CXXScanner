find_package(fmt REQUIRED)
add_library(convertor ClangDeclToCXXModel.cpp ClangDeclToCXXModel.h)
add_library(CXXScanner::convertor ALIAS convertor)
#target_link_libraries(convertor PUBLIC fmt::fmt)
add_library(ast_consumer consumer.cpp consumer.h)
add_library(CXXScanner::ast_consumer ALIAS ast_consumer)
#target_link_libraries(ast_consumer
#        PUBLIC CXXScanner::model)
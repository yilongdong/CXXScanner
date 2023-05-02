add_library(action FrontendAction.cpp FrontendAction.h)
add_library(CXXScanner::action ALIAS action)
target_link_libraries(action
        PUBLIC
        CXXScanner::utility
#        CXXScanner::model
        )
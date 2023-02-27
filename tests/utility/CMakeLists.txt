add_executable(test_log test_log.cpp)
target_link_libraries(test_log
        PRIVATE
        GTest::gtest_main
        CXXScanner::utility)
gtest_discover_tests(test_log)
add_executable(test_connPool test_connPool.cpp)
target_link_libraries(test_connPool
        PRIVATE
        GTest::gtest_main
        CXXScanner::utility
        MongoConnPool
        )
gtest_discover_tests(test_connPool)
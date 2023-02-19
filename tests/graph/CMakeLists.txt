add_executable(test_DAG test_DAG.cpp)
find_package(fmt REQUIRED)
target_link_libraries(test_DAG
        PRIVATE
        GTest::gtest_main
        fmt::fmt)
gtest_discover_tests(test_DAG)
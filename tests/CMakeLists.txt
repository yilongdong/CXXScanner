enable_testing()
include(CTest)
include(GoogleTest)
find_package(GTest CONFIG REQUIRED)
add_executable(test_demo test_demo.cpp)
#target_include_directories(test_demo PUBLIC ${PROJECT_SOURCE_DIR}/include)
target_link_libraries(test_demo
        PUBLIC
#        GTest::gtest
        GTest::gtest_main
#        GTest::gmock
#        GTest::gmock_main
        TUDumperLib)

gtest_discover_tests(test_demo)
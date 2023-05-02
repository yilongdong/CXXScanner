add_executable(CXXScanner main.cpp)
find_package(gflags REQUIRED)
target_link_libraries(CXXScanner PRIVATE CXXScannerLib gflags::gflags)
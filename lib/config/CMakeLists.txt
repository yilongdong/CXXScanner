find_package(toml11 CONFIG REQUIRED)

add_library(config ConfigProvider.cpp ConfigProvider.h)
add_library(CXXScanner::config ALIAS config)
target_link_libraries(config PRIVATE toml11::toml11)

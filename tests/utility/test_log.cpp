#include <gtest/gtest.h>
#include "utility/log.h"

TEST(log, 设置未知日志等级) {
    EXPECT_NO_THROW({ SET_LOGLEVEL("sfsakjfg"); }) << "设置未知日志等级是出现异常";
}

TEST(log, 基本功能) {
    // TODO
//    SET_LOGLEVEL("debug");
//    LOG_INFO("info");
//    LOG_DEBUG("debug");
//    LOG_TRACE("trace");
//    SET_LOGLEVEL("sfsakjfg");
//    LOG_INFO("info after unkown log level");
}

int main(int argc, char* argv[]) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
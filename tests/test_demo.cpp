#include "foo.h"
#include <gtest/gtest.h>

TEST(Demo, add) {
    EXPECT_EQ(3, Add(1, 2));
}
int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
#include "gtest/gtest.h"
#include "config/config.cpp"

using namespace beta;

class ConfigTest : public testing::Test {
};

TEST_F(ConfigTest, get_keyExists) {
    Config &config = Config::getInstance();
    ASSERT_EQ("./providers", config.get("provider_dir"));
}

TEST_F(ConfigTest, get_keyMissing) {
    Config &config = Config::getInstance();
    ASSERT_EQ("", config.get("wef983423hflefn;awef293429"));
}

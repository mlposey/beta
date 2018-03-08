#include "gtest/gtest.h"
#include "config/config.cpp"

using namespace beta;

class ConfigTest : public testing::Test {
};

TEST_F(ConfigTest, getString_keyExists) {
    Config &config = Config::getInstance();
    ASSERT_EQ("./providers", config.getString("provider_dir"));
}

TEST_F(ConfigTest, getString_keyMissing) {
    Config &config = Config::getInstance();
    ASSERT_EQ("", config.getString("wef983423hflefn;awef293429"));
}

// TODO: Test conversions for bytes, kB, MB, and GB.
// That will require not making Config a singleton and passing a filename
// to the constructor instead.
TEST_F(ConfigTest, getBytes) {
    Config &config = Config::getInstance();
    ASSERT_EQ(1000 * 1'000'000, config.getBytes("max_cache_mem"));
}

#include "gtest/gtest.h"
#include "config/config.cpp"

using namespace beta;

class ConfigTest : public testing::Test {
};

TEST_F(ConfigTest, get) {
    Config &config = Config::getInstance();
    ASSERT_EQ("./providers", config.get("provider_dir"));
}

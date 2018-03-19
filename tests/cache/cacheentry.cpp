#include "gtest/gtest.h"
#include "path/path2d.h"
#include "cache/cacheentry.h"

using namespace beta;

class CacheEntryTest : public testing::Test {
};

TEST_F(CacheEntryTest, query) {
    Node origin(3, 5), destination(6, 2);
    auto path = std::make_shared<Path2D>(origin, destination);

    CacheEntry entry(path);
    path_query query = entry.query();

    ASSERT_EQ(origin, query.origin);
    ASSERT_EQ(destination, query.destination);
}
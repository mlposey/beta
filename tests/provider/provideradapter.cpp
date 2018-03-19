#include "gtest/gtest.h"
#include "provider/provideradapter.h"
#include "path/path2d.h"

class MockProvider : public PathProvider {
public:
    std::string name() const { return "MockProvider"; }
    float version() const { return 0.1; }
    std::string author() const { return "Mock"; }

    std::vector<float> findPath(float srcLat, float srcLng, float dstLat, float dstLng) {
        return {3.0, 4.0, 6.0, 9.0};
    }
};

using namespace beta;

class ProviderAdapterTest : public testing::Test {
};

TEST_F(ProviderAdapterTest, findPath) {
    auto mockProvider = std::make_shared<MockProvider>();
    ProviderAdapter adapter(mockProvider);

    Node origin(3.0, 4.0), dest(6.0, 9.0);
    auto expected = std::make_shared<Path2D>(origin ,dest);
    auto actual = adapter.findPath(path_query{origin, dest});

    ASSERT_EQ(expected->nodeCount(), actual->nodeCount());

    for (auto eIt = expected->begin(), aIt = actual->begin();
         eIt != expected->end() && aIt != actual->end();
         ++eIt, ++aIt) {
        ASSERT_EQ(*eIt, *aIt);
    }
}

TEST_F(ProviderAdapterTest, provider) {
    auto mockProvider = std::make_shared<MockProvider>();
    ProviderAdapter adapter(mockProvider);
    ASSERT_EQ(mockProvider, adapter.provider());
}

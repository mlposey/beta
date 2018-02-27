#include <vector>
#include <memory>
#include <utility>
#include <cstdlib>
#include "gtest/gtest.h"
#include "cache/optimizer.cpp"
#include "cache/pathcache.cpp"
#include "cache/cacheentry.h"
#include "path/path2d.h"

using namespace beta;

class PathCacheTest : public testing::Test {
public:
    std::vector<cache_entry> makeEntries();
    size_t entriesSizeBytes(const std::vector<cache_entry> &entries);
};

// Builds entries using the paths defined in figure 3 of the paper
// Efficient Path Query Processing Through Cloud-Based Mapping Services
// by D. Zhang et al.
std::vector<cache_entry> PathCacheTest::makeEntries() {
    std::vector<cache_entry> entries;

    path_query query1{{0.0f, 9.0f}, {9.0f, 2.0f}};
    std::shared_ptr<Path> path1 = std::make_shared<Path2D>(point{0.0f, 9.0f}, point{3.0f, 5.0f});
    path1->add({6.0f, 5.0f});
    path1->add({6.0f, 2.0f});
    path1->add({9.0f, 2.0f});
    entries.push_back({query1, path1});

    path_query query2{{4.0f, 5.0f}, {8.0f, 2.0f}};
    std::shared_ptr<Path> path2 = std::make_shared<Path2D>(point{4.0f, 5.0f}, point{6.0f, 5.0f});
    path2->add({6.0f, 2.0f});
    path2->add({8.0f, 2.0f});
    entries.push_back({query2, path2});

    path_query query3{{3.0f, 0.0f}, {9.0f, 8.0f}};
    std::shared_ptr<Path> path3 = std::make_shared<Path2D>(point{3.0f, 0.0f}, point{3.0f, 5.0f});
    path3->add({4.0f, 8.0f});
    path3->add({9.0f, 8.0f});
    entries.push_back({query3, path3});

    path_query query4{{3.0f, 2.0f}, {7.0f, 8.0f}};
    std::shared_ptr<Path> path4 = std::make_shared<Path2D>(point{3.0f, 2.0f}, point{3.0f, 5.0f});
    path4->add({4.0f, 8.0f});
    path4->add({7.0f, 8.0f});
    entries.push_back({query4, path4});

    path_query query5{{3.0f, 3.0f}, {6.0f, 8.0f}};
    std::shared_ptr<Path> path5 = std::make_shared<Path2D>(point{3.0f, 3.0f}, point{3.0f, 5.0f});
    path5->add({4.0f, 8.0f});
    path5->add({6.0f, 8.0f});
    entries.push_back({query5, path5});

    return entries;
}

size_t PathCacheTest::entriesSizeBytes(const std::vector<cache_entry> &entries) {
    size_t size = 0;
    for (const auto &entry : entries) {
        size += entry.sizeBytes();
    }
    return size;
}


TEST_F(PathCacheTest, constructor_removesSubpaths) {
    std::vector<cache_entry> entries = makeEntries();
    std::pair<size_t, size_t> cases[] = {  // <max bytes size, expected cache size>
        std::make_pair(entriesSizeBytes(entries), 2),
        std::make_pair(1, 0),
        std::make_pair(entriesSizeBytes(entries) * 2, 2)
    };

    for (const auto &tc : cases) {
        PathCache cache(entries, tc.first);
        ASSERT_EQ(tc.second, cache.size());
        ASSERT_EQ(tc.first, cache.capacity());
    }
}

TEST_F(PathCacheTest, keepsOptimalEntries) {
    std::vector<cache_entry> entries = makeEntries();
    cache_entry kept = entries[2];

    PathCache cache(entries, kept.sizeBytes());
    ASSERT_EQ(1, cache.size());
    ASSERT_EQ(kept.sizeBytes(), cache.capacity());

    auto path = cache.find(kept.query);
    ASSERT_TRUE(path != nullptr);
    ASSERT_EQ(path->toString(), kept.path->toString());
}

TEST_F(PathCacheTest, add_preventsOverflow) {
    std::vector<cache_entry> entries = makeEntries();
    std::vector<cache_entry> subentries;
    subentries.push_back(entries[0]);

    PathCache cache(subentries, entriesSizeBytes(subentries));
    ASSERT_THROW(cache.add(entries[2]), std::runtime_error);    
}

#include <vector>
#include <memory>
#include <utility>
#include <cstdlib>
#include "gtest/gtest.h"
#include "cache/optimize/optimizer.cpp"
#include "cache/pathcache.cpp"
#include "cache/cacheentry.h"
#include "path/path2d.h"
#include "path/node.cpp"

using namespace beta;

class PathCacheTest : public testing::Test {
public:
    std::vector<std::shared_ptr<Path>> makePaths();
    size_t entriesSizeBytes(const std::vector<std::shared_ptr<Path>> &entries);
};

// Builds entries using the paths defined in figure 3 of the paper
// Efficient Path Query Processing Through Cloud-Based Mapping Services
// by D. Zhang et al.
std::vector<std::shared_ptr<Path>> PathCacheTest::makePaths() {
    std::vector<std::shared_ptr<Path>> entries;

    std::shared_ptr<Path> path1 = std::make_shared<Path2D>(Node{0.0f, 9.0f}, Node{3.0f, 5.0f});
    path1->add({6.0f, 5.0f});
    path1->add({6.0f, 2.0f});
    path1->add({9.0f, 2.0f});
    entries.push_back(path1);

    std::shared_ptr<Path> path2 = std::make_shared<Path2D>(Node{4.0f, 5.0f}, Node{6.0f, 5.0f});
    path2->add({6.0f, 2.0f});
    path2->add({8.0f, 2.0f});
    entries.push_back(path2);

    std::shared_ptr<Path> path3 = std::make_shared<Path2D>(Node{3.0f, 0.0f}, Node{3.0f, 5.0f});
    path3->add({4.0f, 8.0f});
    path3->add({9.0f, 8.0f});
    entries.push_back(path3);

    std::shared_ptr<Path> path4 = std::make_shared<Path2D>(Node{3.0f, 2.0f}, Node{3.0f, 5.0f});
    path4->add({4.0f, 8.0f});
    path4->add({7.0f, 8.0f});
    entries.push_back(path4);

    std::shared_ptr<Path> path5 = std::make_shared<Path2D>(Node{3.0f, 3.0f}, Node{3.0f, 5.0f});
    path5->add({4.0f, 8.0f});
    path5->add({6.0f, 8.0f});
    entries.push_back(path5);

    return entries;
}

size_t PathCacheTest::entriesSizeBytes(const std::vector<std::shared_ptr<Path>> &entries) {
    size_t size = 0;
    for (const auto &entry : entries) {
        size += CacheEntry(entry).sizeBytes();
    }
    return size;
}

TEST_F(PathCacheTest, add_throwsOverflow) {
    std::vector<std::shared_ptr<Path>> entries = makePaths();
    std::vector<std::shared_ptr<Path>> subentries;
    subentries.push_back(entries[0]);

    PathCache cache(&subentries, entriesSizeBytes(subentries));
    ASSERT_THROW(cache.add(entries[2]), std::overflow_error);    
}

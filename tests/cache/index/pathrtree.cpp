#include <memory>
#include "gtest/gtest.h"
#include "cache/index/pathrtree.cpp"
#include "path/path.h"
#include "path/path2d.h"

using namespace beta;

class PathRTreeTest : public testing::Test {
public:
    std::shared_ptr<Path> buildPath();
    bool equal(std::shared_ptr<Path> a, std::shared_ptr<Path> b);
};

std::shared_ptr<Path> PathRTreeTest::buildPath() {
    std::shared_ptr<Path> path = std::make_shared<Path2D>(Node{0.0f, 9.0f}, Node{3.0f, 5.0f});    
    path->push_back({6.0f, 5.0f});
    path->push_back({6.0f, 2.0f});
    path->push_back({9.0f, 2.0f});
    return path;
}

bool PathRTreeTest::equal(std::shared_ptr<Path> a, std::shared_ptr<Path> b) {
    if (a->nodeCount() != b->nodeCount()) return false;
    for (auto aIt = a->begin(), bIt = b->begin(); aIt != a->end() && bIt != b->end(); aIt++, bIt++) {
        if (!(*aIt == *bIt)) return false;
    }
    return true;
}

TEST_F(PathRTreeTest, find) {
    std::unique_ptr<PathIndex> paths = std::make_unique<PathRTree>();
    auto expected = buildPath();
    paths->add(expected);

    path_query query{*(expected->begin()), *(expected->end() - 1)};
    auto actual = paths->find(query);
    ASSERT_FALSE(actual == nullptr);
    ASSERT_TRUE(equal(expected, actual));

    query.destination = Node(0.0f, 0.0f);
    ASSERT_EQ(nullptr, paths->find(query));
}

TEST_F(PathRTreeTest, remove) {
    std::unique_ptr<PathIndex> pathIndex = std::make_unique<PathRTree>();
    auto path = buildPath();
    pathIndex->add(path);
    path_query query{*(path->begin()), *(path->end() - 1)};

    ASSERT_FALSE(pathIndex->find(query) == nullptr);

    pathIndex->remove(path);
    ASSERT_TRUE(pathIndex->find(query) == nullptr);        
}

TEST_F(PathRTreeTest, remove_single) {
    std::unique_ptr<PathIndex> pathIndex = std::make_unique<PathRTree>();
    auto path1 = buildPath();
    auto path2 = buildPath();

    pathIndex->add(path1);
    pathIndex->add(path2);
    path_query query{*(path1->begin()), *(path1->end() - 1)};

    ASSERT_FALSE(pathIndex->find(query) == nullptr);
    pathIndex->remove(path1);
    ASSERT_FALSE(pathIndex->find(query) == nullptr);        
}

TEST_F(PathRTreeTest, contains_path) {
    std::unique_ptr<PathIndex> paths = std::make_unique<PathRTree>();
    auto path = buildPath();
    paths->add(path);

    ASSERT_TRUE(paths->contains(path));
    auto extended = buildPath();
    extended->push_back(Node(0.0f, 0.0f));
    ASSERT_FALSE(paths->contains(extended));
}

TEST_F(PathRTreeTest, contains_query) {
    std::unique_ptr<PathIndex> paths = std::make_unique<PathRTree>();
    auto path = buildPath();
    paths->add(path);

    path_query query{*(path->begin()), *(path->end() - 1)};
    ASSERT_TRUE(paths->contains(query));
    query.destination = Node(0.0f, 0.0f);    
    ASSERT_FALSE(paths->contains(query));
}

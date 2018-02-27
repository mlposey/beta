#include "gtest/gtest.h"
#include "path/path2d.cpp"

class Path2DTest : public testing::Test {
};

using namespace beta;

TEST_F(Path2DTest, nodeCount) {
    Path2D path({0.0f, 9.0f}, {3.0f, 5.0f});
    ASSERT_EQ(2, path.nodeCount());

    path.add({2.0f, 1.0f});
    ASSERT_EQ(3, path.nodeCount());
}

TEST_F(Path2DTest, euclideanDistance) {
    Path2D path({0.0f, 9.0f}, {3.0f, 5.0f});
    ASSERT_EQ(5, path.euclideanDistance());
}

TEST_F(Path2DTest, realDistance) {
    point a{0.0f, 9.0f}, b{3.0f, 5.0f};    
    Path2D path(a, b);
    ASSERT_EQ(path.euclideanDistance(), path.realDistance());
    
    point c{0.0f, 0.0f};
    path.add(c);

    double expectedDistance = a.distanceTo(b) + b.distanceTo(c);
    ASSERT_EQ(expectedDistance, path.realDistance());
}

TEST_F(Path2DTest, contains) {
    point a{0.0f, 9.0f}, b{3.0f, 5.0f};
    Path2D path(a, b);
    path.add({2.9f, 9.9f});

    ASSERT_TRUE(path.contains(a));
    ASSERT_TRUE(path.contains(b));
    ASSERT_FALSE(path.contains({0.0f, 0.0f}));

    point midpoint{(a.lat + b.lat) / 2, (a.lng + b.lng) / 2};
    ASSERT_TRUE(path.contains(midpoint)); 
}

TEST_F(Path2DTest, constIterators) {
    point a{0.0f, 9.0f}, b{3.0f, 5.0f};
    Path2D path(a, b);

    auto it = path.begin();

    ASSERT_EQ(a, *it++);
    ASSERT_EQ(b, *it++);
    ASSERT_EQ(path.end(), it);
}

#include <utility>
#include <vector>
#include "gtest/gtest.h"
#include "path/node.h"

using namespace beta;

class NodeTest : public testing::Test {
public:
    NodeTest()
        : a(5.0f, 6.0f)
        , c(2.0f, 6.0f)
    {}

protected:
    const Node a;
    const Node c;
};

TEST_F(NodeTest, distanceTo) {
    // [<[origin, destination], distance>...]
    std::pair<std::vector<Node>, double> testCases[] = {
        std::make_pair(std::vector<Node>{a, c}, 3.0),
        std::make_pair(std::vector<Node>{a, a}, 0.0),
        std::make_pair(std::vector<Node>{c, c}, 0.0)
    };

    for (auto testCase : testCases) {
        auto distance = testCase.first[0].distanceTo(testCase.first[1]);
        ASSERT_EQ(testCase.second, distance);
    }
}

TEST_F(NodeTest, isBetween) {
    // [<node, is between this->a and this->c>...]
    std::pair<Node, bool> testCases[] = {
        std::make_pair(Node(4.0f, 6.0f), true),
        std::make_pair(Node(4.0f, 6.001f), false),
        std::make_pair(a, true),
        std::make_pair(c, true),
        std::make_pair(Node(6.0f, 6.0f), false)
    };

    for (auto testCase : testCases) {
        Node &b = testCase.first;
        ASSERT_EQ(testCase.second, b.isBetween(a, c))
            << b.str() << " should " << (testCase.second ? "" : "not ")
            << "be between " << a.str() << " and " << c.str();
    }
}

TEST_F(NodeTest, operatorEquals) {
    Node b(5.0f, 6.0f);
    ASSERT_EQ(a, b);
    ASSERT_FALSE(a == c);
    ASSERT_FALSE(Node(5.001f, 6.0f) == b);
}

TEST_F(NodeTest, latitude) {
    ASSERT_DOUBLE_EQ(5.0f, a.latitude());
}

TEST_F(NodeTest, longitude) {
    ASSERT_DOUBLE_EQ(6.0f, a.longitude());
}
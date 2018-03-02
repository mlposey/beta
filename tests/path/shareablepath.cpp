#include <vector>
#include <memory>
#include "gtest/gtest.h"
#include "path/path2d.h"
#include "path/shareablepath.cpp"

using namespace beta;

struct shareable_case {
    path_query query;
    bool answerableFromPath;
};

class ShareablePathTest : public testing::Test {
public:
    std::shared_ptr<Path> buildPath();
    void testShare(ShareablePath path, shareable_case tc);
};

std::shared_ptr<Path> ShareablePathTest::buildPath() {
    std::shared_ptr<Path> path = std::make_shared<Path2D>(Node{0.0f, 9.0f}, Node{3.0f, 5.0f});    
    path->add({6.0f, 5.0f});
    path->add({6.0f, 2.0f});
    path->add({9.0f, 2.0f});
    return path;
}

TEST_F(ShareablePathTest, canAnswer) {
    ShareablePath path(buildPath());
    path_query answerableQuery{{4.0f, 5.0f}, {8.0f, 2.0f}};
    path_query unanswerableQuery{{4.0f, 5.0f}, {7.0f, 7.0f}};

    ASSERT_TRUE(path.canShare(answerableQuery));
    ASSERT_FALSE(path.canShare(unanswerableQuery));
}

TEST_F(ShareablePathTest, sharingAbility) {
    ShareablePath path(buildPath());
    path_query answerableQuery{{4.0f, 5.0f}, {8.0f, 2.0f}};
    path_query unanswerableQuery{{4.0f, 5.0f}, {7.0f, 7.0f}};
    std::vector<path_query> queries{answerableQuery, unanswerableQuery};

    ASSERT_EQ(1, path.sharingAbility(queries));
}

TEST_F(ShareablePathTest, share) {
    ShareablePath path(buildPath());
    std::vector<shareable_case> testCases;
    // The origin and destination follow the order of the path, e.g.,
    // Path: o -> v1 -> v2 -> d
    // Query: (v1, d)
    testCases.push_back({{{4.0f, 5.0f}, {8.0f, 2.0f}}, true});

    // The origin and destination are in reverse order of the path, e.g.,
    // Path: o -> v1 -> v2 -> d
    // Query: (v2, o)
    testCases.push_back({{{8.0f, 2.0f}, {4.0f, 5.0f}}, true});

    // The query does not lie on the path, e.g.,
    // Path: o1 -> v1 -> v2 -> d1
    // Query: (v5, v9)
    testCases.push_back({{{8.0f, 6.0f}, {9.0f, 3.0f}}, false});

    // The query has only a single point on the path, e.g.,
    // Path: o1 -> v1 -> v2 -> d1
    // Query: (v1, v9)
    testCases.push_back({{{4.0f, 5.0f}, {5.0f, 6.0f}}, false});    

    for (auto &tc : testCases) testShare(path, tc);
}

void ShareablePathTest::testShare(ShareablePath path, shareable_case testCase) {
    std::shared_ptr<Path> queryPath = path.share(testCase.query);
    
    if (testCase.answerableFromPath) {
        Node origin = testCase.query.origin, destination = testCase.query.destination;
        ASSERT_TRUE(queryPath != nullptr);
        ASSERT_TRUE(queryPath->contains(origin));
        ASSERT_TRUE(queryPath->contains(destination));
        ASSERT_EQ(origin.distanceTo(destination), queryPath->euclideanDistance());
    } else {
        ASSERT_TRUE(queryPath == nullptr) << "failed check for case " << testCase.query.str()
                                          << "; expected no path, found " << queryPath->str();
    }
}

TEST_F(ShareablePathTest, share_pathMatch) {
    auto path = buildPath();
    ShareablePath shared(path);

    path_query req{{3.0f, 5.0f}, {6.0f, 2.0f}};
    std::shared_ptr<Path> res = shared.share(req);

    ASSERT_FALSE(res == nullptr);
    ASSERT_EQ(3, res->nodeCount());

    Node v1{3.0f, 5.0f}, v2{6.0f, 5.0f}, v3{6.0f, 2.0f};
    auto it = res->begin();
    ASSERT_EQ(v1, *it++);
    ASSERT_EQ(v2, *it++);
    ASSERT_EQ(v3, *it++);
}

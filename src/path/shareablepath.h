#pragma once
#include <memory>
#include <vector>
#include <deque>
#include "path.h"

namespace beta {

struct path_query {
    std::string str() const {
        return "[" + origin.str() + ", " + destination.str() + "]";
    }
    Node origin;
    Node destination;
};

#define PATH_QUERY_SIZE_BYTES 16

class ShareablePath  {
public:
    explicit ShareablePath(std::shared_ptr<Path> path);
    size_t sharingAbility(const std::vector<path_query> &queries) const;
    bool canShare(const path_query &query) const;
    std::shared_ptr<Path> share(const path_query &query) const;

private:
    class PathBuilder;

    std::vector<Node> buildSubpath(const path_query &query) const;
    std::shared_ptr<Path> path;
};

class ShareablePath::PathBuilder {
public:
    PathBuilder() : isBuildDirectionForward(true) {}
    void setForward() { isBuildDirectionForward = true; }
    void setReverse() { isBuildDirectionForward = false; }

    void add(const Node &node);
    std::shared_ptr<Path> build();

private:
    std::deque<Node> nodes;
    bool isBuildDirectionForward;
};

}  // namespace beta
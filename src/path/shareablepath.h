#pragma once
#include <memory>
#include <vector>
#include <deque>
#include "path.h"

namespace beta {

/** Defines a request for a path using its starting and ending nodes */
struct path_query {
    /** Returns a string representation of the query */
    std::string str() const {
        return "[" + origin.str() + ", " + destination.str() + "]";
    }
    Node origin;
    Node destination;
};

// The expected size of a path_query object in bytes
#define PATH_QUERY_SIZE_BYTES NODE_SIZE_BYTES * 2

/** Defines a path whose subpaths can potentially answer path queries */
class ShareablePath  {
public:
    explicit ShareablePath(std::shared_ptr<Path> path);
    /** Computes a score that represents the path's ability to benefit the query set */
    size_t sharingAbility(const std::vector<path_query> &queries) const;
    /** Determines if the path can provide a subpath that answers a query */
    bool canShare(const path_query &query) const;
    /** Returns a subpath from the path that answers the query -- or nullptr if no subpath exists */
    std::shared_ptr<Path> share(const path_query &query) const;

private:
    class PathBuilder;
    
    std::shared_ptr<Path> path;
};

/** Uses the nodes in existing paths to build subpaths which answer queries */
class ShareablePath::PathBuilder {
public:
    /** Creates a builder that searches path for an answer to query */
    PathBuilder(const std::shared_ptr<Path> &path, const path_query &query);

    /**
     * Builds a path that satisfies the target query
     * @returns the subpath that satisfies the query or nullptr if the query is not satisfiable
     */
    std::shared_ptr<Path> build();

private:
    Path::const_iterator findSubpathStart();
    void buildSubpath(Path::const_iterator subpathOrigin);
    void add(const Node &node);

    const path_query &query;
    const std::shared_ptr<Path> &masterPath;
    std::deque<Node> subpathNodes;

    bool isBuildDirectionForward;  // Are we appending or prepending nodes to the subpath?
    bool foundOrigin;
    bool foundDestination;
};

}  // namespace beta
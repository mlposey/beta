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

    std::vector<Node> buildSubpath(const path_query &query) const;
    std::shared_ptr<Path> path;
};

/**
 * Builds subpaths for ShareablePath
 * 
 * PathBuilder allows paths to be built in forward order (by appending nodes) as well as
 * in reverse (by prepending).
 */
class ShareablePath::PathBuilder {
public:
    /** Creates a builder that constructs a path in a forward direction */
    PathBuilder() : isBuildDirectionForward(true) {}

    /** Instructs the builder to add new nodes to the end of the path */
    void setForward() { isBuildDirectionForward = true; }
    /** Instructs the builder to add new nodes to the beginning of the path */
    void setReverse() { isBuildDirectionForward = false; }

    /**
     * Adds a node to the path
     * The ordering of node additions is dictated by the methods setForward and setReverse.
     */
    void add(const Node &node);
    
    /** Builds the final Path representation of the node collection */
    std::shared_ptr<Path> build();

private:
    std::deque<Node> nodes;
    bool isBuildDirectionForward;
};

}  // namespace beta
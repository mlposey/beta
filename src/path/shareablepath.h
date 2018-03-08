#pragma once
#include <memory>
#include <vector>
#include <deque>
#include "path.h"
#include "pathquery.h"

namespace beta {

/** Defines a path whose subpaths can potentially answer path queries */
class ShareablePath  {
public:
    explicit ShareablePath(std::shared_ptr<Path> path);
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
    std::shared_ptr<Path> subpath;

    bool isBuildDirectionForward;  // Are we appending or prepending nodes to the subpath?
    bool foundOrigin;
    bool foundDestination;
};

}  // namespace beta
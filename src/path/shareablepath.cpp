#include <cstdlib>
#include "shareablepath.h"
#include "path2d.h"

namespace beta {

ShareablePath::ShareablePath(std::shared_ptr<Path> path): path(path) {}

bool ShareablePath::canShare(const path_query &query) const {
    return path->contains(query.origin) && path->contains(query.destination);
}

std::shared_ptr<Path> ShareablePath::share(const path_query &query) const {
    PathBuilder builder(path, query);
    return builder.build();
}

ShareablePath::PathBuilder::PathBuilder(const std::shared_ptr<Path> &path, const path_query &query)
    : query(query)
    , masterPath(path)
    , isBuildDirectionForward(true)
    , foundOrigin(false)
    , foundDestination(false)
{}

std::shared_ptr<Path> ShareablePath::PathBuilder::build() {
    subpath = std::make_shared<Path2D>();
    auto subpathStart = findSubpathStart();
    if (subpathStart == masterPath->end()) return nullptr;

    buildSubpath(subpathStart);
    return (foundOrigin && foundDestination) ? subpath : nullptr;
}

Path::const_iterator ShareablePath::PathBuilder::findSubpathStart() {
    for (auto it = masterPath->begin(); it != std::prev(masterPath->end()); it++) {
        if (query.origin.isBetween(*it, *std::next(it))) {
            isBuildDirectionForward = true;
            foundOrigin = true;
            add(query.origin);
            return query.origin == *std::next(it) ? std::next(it, 2) : std::next(it);
        } else if (query.destination.isBetween(*it, *std::next(it))) {
            isBuildDirectionForward = false;
            foundDestination = true;
            add(query.destination);
            return query.destination == *std::next(it) ? std::next(it, 2) : std::next(it);            
        }
    }
    return masterPath->end();
}

void ShareablePath::PathBuilder::buildSubpath(Path::const_iterator it) {
    for (; it != std::prev(masterPath->end()); ++it) {
        if (!foundOrigin && query.origin.isBetween(*it, *std::next(it))) {
            if (query.origin == *std::next(it)) add(*it);
            add(query.origin);
            foundOrigin = true;
            return;
        } else if (!foundDestination && query.destination.isBetween(*it, *std::next(it))) {
            if (query.destination == *std::next(it)) add(*it);            
            add(query.destination);
            foundDestination = true;
            return;
        } else {
            add(*it);
        }
    }
}

void ShareablePath::PathBuilder::add(const Node &node) {
    if (isBuildDirectionForward) subpath->push_back(node);
    else subpath->push_front(node);
}

}  // namespace beta

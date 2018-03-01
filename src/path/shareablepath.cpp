#include <cstdlib>
#include "shareablepath.h"
#include "path2d.h"

namespace beta {

ShareablePath::ShareablePath(std::shared_ptr<Path> path): path(path) {}

size_t ShareablePath::sharingAbility(const std::vector<path_query> &queries) const {
    size_t sa = 0;
    for (const auto &query : queries) {
        if (canShare(query)) sa++;
    }
    return sa;
}

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
    auto subpathStart = findSubpathStart();
    if (subpathStart == masterPath->end()) return nullptr;

    buildSubpath(subpathStart);
    if (!foundDestination) return nullptr;

    std::shared_ptr<Path> path = std::make_shared<Path2D>(subpathNodes[0], subpathNodes[1]);
    for (int i = 2; i < subpathNodes.size(); i++) {
        path->add(subpathNodes[i]);
    }
    return path;
}

Path::const_iterator ShareablePath::PathBuilder::findSubpathStart() {
    for (auto it = masterPath->begin(); it != masterPath->end() - 1; it++) {
        if (query.origin.isBetween(*it, *(it+1))) {
            isBuildDirectionForward = true;
            foundOrigin = true;
            add(query.origin);
            return it++;
        } else if (query.destination.isBetween(*it, *(it+1))) {
            isBuildDirectionForward = false;
            foundDestination = true;
            add(query.destination);
            return it++;
        }
    }
    return masterPath->end();
}

void ShareablePath::PathBuilder::buildSubpath(Path::const_iterator it) {
    for (; it != masterPath->end() - 1; it++) {
        if (!foundOrigin && query.origin.isBetween(*it, *(it+1))) {
            add(query.origin);
            foundOrigin = true;
            return;
        } else if (!foundDestination && query.destination.isBetween(*it, *(it+1))) {
            add(query.destination);
            foundDestination = true;
            return;
        } else {
            add(*it);
        }
    }
}

void ShareablePath::PathBuilder::add(const Node &node) {
    if (isBuildDirectionForward) subpathNodes.push_back(node);
    else subpathNodes.push_front(node);
}

}  // namespace beta
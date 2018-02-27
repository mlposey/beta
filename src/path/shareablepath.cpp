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
    PathBuilder pathBuilder;
    bool foundStart = false;
    bool hasCompletePath = false;
    
    for (auto it = path->begin(); it != path->end() && !hasCompletePath; it++) {
        if (!foundStart) {
            if (query.origin.isBetween(*it, *(it+1))) {
                pathBuilder.setForward();
                pathBuilder.add(query.origin);
                foundStart = true;
            } else if (query.destination.isBetween(*it, *(it+1))) {
                pathBuilder.setReverse();
                pathBuilder.add(query.destination);
                foundStart = true;
            }
        } else {
            if (query.origin.isBetween(*it, *(it+1))) {
                pathBuilder.add(query.origin);
                hasCompletePath = true;
            } else if (query.destination.isBetween(*it, *(it+1))) {
                pathBuilder.add(query.destination);
                hasCompletePath = true;
            } else {
                pathBuilder.add(*it);
            }
        }        
    }
    return hasCompletePath ? pathBuilder.build() : nullptr;
}

void ShareablePath::PathBuilder::add(const point &p) {
    if (isBuildDirectionForward) points.push_back(p);
    else points.push_front(p);
}

std::shared_ptr<Path> ShareablePath::PathBuilder::build() {
    if (points.size() < 2) return nullptr;

    std::shared_ptr<Path> path = std::make_shared<Path2D>(points[0], points[1]);
    for (int i = 2; i < points.size(); i++) {
        path->add(points[i]);
    }
    return path;
}

}  // namespace beta
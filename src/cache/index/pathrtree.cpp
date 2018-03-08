#include <unordered_set>
#include <vector>
#include <boost/function_output_iterator.hpp>
#include "pathrtree.h"
#include "path/shareablepath.h"

namespace beta {

void PathRTree::add(std::shared_ptr<Path> path) {
    auto last = std::prev(path->end());
    for (auto it = path->begin(); it != last;) {
        point a = makePoint(*it++);
        point b = makePoint(*it);
        tree.insert(std::make_pair(segment(a, b), path));
    }
}

void PathRTree::remove(std::shared_ptr<Path> path) {
    std::vector<path_ref> toDelete;
    for (auto it = path->begin(); it != path->end(); it++) {
        point p = makePoint(*it);
        tree.query(bgi::intersects(p), boost::make_function_output_iterator([&](path_ref const& ref){
            if (ref.second.get() == path.get()) {
                toDelete.push_back(ref);
                return;
            }
        }));
    }
    for (auto ref : toDelete) tree.remove(ref);
}

std::shared_ptr<Path> PathRTree::find(const path_query &query) const {
    std::unordered_set<Path*> candidates;
    handleIntersectingPaths(makePoint(query.origin), [&](auto path) {
        candidates.insert(path.get());
    });

    std::shared_ptr<Path> match;
    handleIntersectingPaths(makePoint(query.destination), [&](auto path) {
        if (candidates.find(path.get()) != candidates.end()) {
            ShareablePath spath(path);
            match = spath.share(query);
            return;
        }
    });
    return match;
}

void PathRTree::handleIntersectingPaths(point p, std::function<void(std::shared_ptr<Path>)> fn) const {
    tree.query(bgi::intersects(p), boost::make_function_output_iterator([&fn](path_ref const& ref){
        fn(ref.second);
    }));
}

bool PathRTree::contains(std::shared_ptr<Path> path) const {
    return contains(path_query{*(path->begin()), *(std::prev(path->end()))});
}

bool PathRTree::contains(const path_query &query) const {
    return find(query) != nullptr;
}

size_t PathRTree::sizeBytes(std::shared_ptr<Path> path) const {
    return (path->nodeCount() - 1) * sizeof(path_ref);
}

}  // namespace beta

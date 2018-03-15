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
    std::vector<path_ref> toDelete = collectReferences(path);
    for (const path_ref &ref : toDelete) tree.remove(ref);
}

std::vector<path_ref> PathRTree::collectReferences(std::shared_ptr<Path> path) {
    std::vector<path_ref> refs;
    refs.reserve(path->nodeCount() - 1);

    auto pathEnd = std::prev(path->end()); // The last point won't have an entry.
    for (auto pathIt = path->begin(); pathIt != pathEnd; ++pathIt) {
        point p = makePoint(*pathIt);
        for (auto treeIt = tree.qbegin(bgi::intersects(p)); treeIt != tree.qend(); ++treeIt) {
            if (treeIt->second.get() == path.get()) {
                refs.push_back(*treeIt);
                break;
            }
        }
    }
    return refs;
}

std::shared_ptr<Path> PathRTree::find(const path_query &query) const {
    std::unordered_set<Path*> candidates;
    handleIntersecting(query.origin, [&](const std::shared_ptr<Path> &path) {
        candidates.insert(path.get());
        return true;
    });

    std::shared_ptr<Path> match;
    handleIntersecting(query.destination, [&](const std::shared_ptr<Path> &path) {
        if (candidates.find(path.get()) != candidates.end()) {
            ShareablePath spath(path);
            match = spath.share(query);
        }
        return match == nullptr;
    });
    return match;
}

void PathRTree::handleIntersecting(const Node &n, const path_handler &handler) const {
    point p = makePoint(n);
    for (auto it = tree.qbegin(bgi::intersects(p)); it != tree.qend(); ++it) {
        if (!handler(it->second)) break;
    }
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

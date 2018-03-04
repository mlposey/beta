#include <stdexcept>
#include "pathcache.h"
#include "optimizer.h"

namespace beta {

PathCache::PathCache(size_t capacityBytes)
    : PathCache(nullptr, capacityBytes)
{}

PathCache::PathCache(std::vector<std::shared_ptr<Path>> *paths, size_t capacityBytes)
    : capacityBytes(capacityBytes)
    , currentSizeBytes(0) {
    if (capacityBytes == 0) throw std::runtime_error("path cache size must be > 0");

    if (!paths) {
        return;
    } else if (paths->size() == 1) {
        add((*paths)[0]);
    } else if (paths->size() > 1) {
        cache = makeEntries(paths);
        Optimizer optimizer(cache);
        currentSizeBytes = optimizer.optimize(capacityBytes);
    }
}

std::vector<CacheEntry> PathCache::makeEntries(std::vector<std::shared_ptr<Path>> *paths) {
    std::vector<CacheEntry> entries;
    for (std::shared_ptr<Path> path : *paths) {
        entries.push_back(std::move(path));
    }
    return entries;
}

void PathCache::add(std::shared_ptr<Path> path) {
    // TODO: Make sure entry path and query isn't already there.
    CacheEntry entry(path);
    size_t entrySize = entry.sizeBytes();
    if (currentSizeBytes + entrySize <= capacityBytes) {
        cache.push_back(entry);
        currentSizeBytes += entrySize;
    } else {
        // TODO: Reoptimize the cache.
        throw std::runtime_error("cache size limit reached; rejecting query " + entry.query().str());
    }
}

std::shared_ptr<Path> PathCache::find(const path_query &query) const {
    // TODO: Use a map to check for an exact match on the query.
    // TODO: Use geographic tags to narrow search space.
    for (const auto &entry : cache) {
        ShareablePath path(entry.path());
        auto subpath = path.share(query);
        if (subpath) return subpath;
    }
    return nullptr;
}

}  // namespace beta

#include <stdexcept>
#include "pathcache.h"
#include "optimizer.h"

namespace beta {

PathCache::PathCache(const std::vector<cache_entry> &entries, size_t capacityBytes)
    : capacityBytes(capacityBytes) {
    if (capacityBytes == 0) throw std::runtime_error("path cache size must be > 0");

    if (entries.size() == 1) {
        cache.push_back(entries[0]);
    } else if (entries.size() > 1) {
        cache = entries;
        Optimizer optimizer(cache);
        currentSizeBytes = optimizer.optimize(capacityBytes);
    }
}

void PathCache::add(const cache_entry &uniqueEntry) {
    // TODO: Make sure entry path and query isn't already there.
    size_t entrySize = uniqueEntry.sizeBytes();
    if (currentSizeBytes + entrySize <= capacityBytes) {
        cache.push_back(uniqueEntry);
        currentSizeBytes += entrySize;
    } else {
        // TODO: Reoptimize the cache.
        throw std::runtime_error("cache size limit reached; rejecting query " + uniqueEntry.query.toString());
    }
}

std::shared_ptr<Path> PathCache::find(const path_query &query) const {
    // TODO: Use a map to check for an exact match on the query.
    // TODO: Use geographic tags to narrow search space.
    for (const auto &entry : cache) {
        ShareablePath path(entry.path);
        auto subpath = path.share(query);
        if (subpath) return subpath;
    }
    return nullptr;
}

}  // namespace beta

#pragma once
#include <vector>
#include "cacheentry.h"

namespace beta {

/** Creates an optimal path cache with respect to memory consumption */
class Optimizer {
public:
    /**
     * Creates an Optimzer instance that will operate on a populated cache
     * One should assume that any value in cache prior to calling this
     * method may either be moved or deleted after Optimizer construction.
     */
    explicit Optimizer(std::vector<CacheEntry> &cache);

    /**
     * Produces an optimal cache whose size is not greater than capacityBytes
     * @returns the size of the cache in bytes after optimization
     */
    size_t optimize(size_t capacityBytes);

private:
    struct ranked_entry;

    /** Builds and ranks a set of cache entries that are not subpaths of one another */
    std::vector<ranked_entry> rankCacheEntries();
    /** Removes suboptimal entries from the collection until the total size is less than capacityBytes */
    void prune(std::vector<ranked_entry> &rankedEntries);

    /** Sorts the cache (in descending order) by the real distance of each path */
    void sortCacheByRealDistDesc();
    /** Sorts the ranked collection (in descending order) by the sharing ability per node of the paths */
    void sortBySAPNDesc(std::vector<ranked_entry> &rankedEntries);

    // The max allowed size of the cache in bytes
    size_t capacityBytes;
    // The current size of the cache in bytes
    size_t currentSizeBytes;
    // The cache to be optimized
    std::vector<CacheEntry> &cache;
};

/** Pairs a cache entry with the sharing ability per node of its path */
struct Optimizer::ranked_entry {
    CacheEntry *entry;
    double sharingAbilityPerNode;
};

}  // namespace beta

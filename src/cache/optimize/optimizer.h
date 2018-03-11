#pragma once
#include <list>
#include <memory>
#include <unordered_map>
#include "cache/cacheentry.h"
#include "cache/index/pathindex.h"

namespace beta {

/**
 * Optimizes a cache and associated index with respect to memory consumption
 * 
 * Optimizer is based heavily on the paper "Efficient Path Query Processing Through Cloud-Based Mapping Services"
 * Citation:
 *      D. Zhang, Y. Liu, A. Liu, X. Mao and Q. Li, "Efficient Path Query Processing Through Cloud-Based Mapping Services," in IEEE Access, vol. 5, pp. 12963-12973, 2017.
 *      doi: 10.1109/ACCESS.2017.2725308
 */
class Optimizer {
public:
    /**
     * Creates an Optimzer instance that will operate on a populated cache
     * The values of cache and index will be modified.
     */
    Optimizer(std::list<CacheEntry> &cache, std::shared_ptr<PathIndex> index);

    /**
     * Produces a space-optimal cache and index
     * @param capacityBytes the maximum size of the optimized cache
     * @param currentSizeBytes the current size of the cache
     */
    void optimize(size_t capacityBytes, size_t &currentSizeBytes);

private:
    /**
     * Removes paths from the cache and index that are duplicates or subpaths
     * This requires the cache to be sorted by real distance descending.
     */
    void removeSubpaths();

    /** Sorts the cache (in descending order) by the real distance of each path */
    void sortCacheByRealDistDesc();
    /** Sorts the cache (in descending order) by the sharing ability per node of the paths */
    void sortCacheBySAPNDesc();

    /**
     * Removes suboptimal paths from the cache until the memory usage is acceptable
     * This requires the cache to be sorted by sharing ability per node descencing.
     */
    void prune(size_t capacityBytes, size_t &currentSizeBytes);
    /** Deletes cache and index elements from begin until the end */
    void chop(std::list<CacheEntry>::iterator begin);

    std::list<CacheEntry> &cache;
    std::shared_ptr<PathIndex> index;

    // Maps a path to its sharing ability per node
    std::unordered_map<Path*, float> pathRanks;
};

}  // namespace beta

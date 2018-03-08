#pragma once
#include <cstddef>
#include <memory>
#include <vector>
#include <list>
#include "path/path.h"
#include "path/shareablepath.h"
#include "cacheentry.h"
#include "cache/index/pathrtree.h"

namespace beta {

/**
 * Holds a collection of queryable paths in memory
 *
 * PathCache contains a collection of shareable paths. I.e., given a node origin
 * and destination, the cache will look for paths that contain a minimal route
 * that answers the query.
 * 
 * The cache is memory sensitive and will prune suboptimal paths from the
 * collection if a capacity is reached.
 */
class PathCache {
public:
    /**
     * Constructs a PathCache instance
     * @param capacityBytes The maximum size of the cache during its lifetime
     * @throws std::runtime_error if capacityBytes is not greater than zero
     */
    explicit PathCache(size_t capacityBytes);

    /**
     * Constructs a PathCache instance
     * 
     * @param paths The collection to start the cache with; may be nullptr
     * @param capacityBytes The maximum size of the cache during its lifetime
     * @throws std::overflow_error if the memory capacity is exceeded
     * @throws std::runtime_error if capacityBytes is not greater than zero
     */
    PathCache(std::vector<std::shared_ptr<Path>> *paths, size_t capacityBytes);

    /**
     * Adds a path to the cache
     * @throws std::overflow_error if the memory capacity is exceeded and the cache
     *                             cannot be optimized
     */
    void add(std::shared_ptr<Path> path);
    /** Retrieves a path that answers the given query or nullptr if no such path exists */
    std::shared_ptr<Path> find(const path_query &query) const;
    
    /** Returns the maximum allowed size (in bytes) of the cache */
    size_t capacity() const { return capacityBytes; }

private:
    void optimizeCache();

    const size_t capacityBytes;
    size_t currentSizeBytes;

    std::list<CacheEntry> cache;
    std::shared_ptr<PathIndex> pathIndex;
};

inline std::shared_ptr<Path> PathCache::find(const path_query &query) const {
    // TODO: Use geographic tags to narrow search space.
    return pathIndex->find(query);
}

}  // namespace beta

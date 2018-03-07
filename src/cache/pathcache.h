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
 * PathCache contains a collection of paths that are optimally shareable. I.e., given
 * a node origin and destination, one should hope to retrieve a path between them
 * that is based on a master cached path.
 */
class PathCache {
public:
    /**
     * Constructs a PathCache instance
     * @param capacityBytes The maximum size of the cache during its lifetime
     */
    explicit PathCache(size_t capacityBytes);

    /**
     * Constructs a PathCache instance
     * 
     * @param entries The collection to start the cache with. This variable will
     *                be modified by the constructor.
     * @param capacityBytes The maximum size of the cache during its lifetime
     * @throws std::overflow_error if the memory capacity is exceeded
     */
    PathCache(std::vector<std::shared_ptr<Path>> *paths, size_t capacityBytes);

    /**
     * Adds a path to the cache
     * @throws std::overflow_error if the memory capacity is exceeded
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

}  // namespace beta

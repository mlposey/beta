#pragma once
#include <cstddef>
#include <memory>
#include <vector>
#include "path/path.h"
#include "path/shareablepath.h"
#include "cacheentry.h"

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
     * @param entries The collection to start the cache with. This variable will
     *                be modified by the constructor.
     * @param capacityBytes The maximum size of the cache during its lifetime
     */
    PathCache(std::vector<cache_entry> &entries, size_t capacityBytes);

    /** Adds an entry to the cache */
    void add(const cache_entry &entry);
    /** Retrieves a path that answers the given query or nullptr if no such path exists */
    std::shared_ptr<Path> find(const path_query &query) const;
    
    /** Returns the maximum allowed size (in bytes) of the cache */
    size_t capacity() const { return capacityBytes; }
    /** Returns the current size (in bytes) of the cache */
    size_t size() const { return cache.size(); }

private:
    const size_t capacityBytes;
    size_t currentSizeBytes;
    std::vector<cache_entry> cache;
};

}  // namespace beta

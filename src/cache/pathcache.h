#pragma once
#include <cstddef>
#include <memory>
#include <vector>
#include "path/path.h"
#include "path/shareablepath.h"
#include "cacheentry.h"

namespace beta {

class PathCache {
public:
    PathCache(const std::vector<cache_entry> &entries, size_t capacityBytes);
    void add(const cache_entry &entry);
    std::shared_ptr<Path> find(const path_query &query) const;
    
    // TODO: void save(std::shared_ptr<CacheSaver> saver);

    size_t capacity() const { return capacityBytes; }
    size_t size() const { return cache.size(); }

private:
    const size_t capacityBytes;
    size_t currentSizeBytes;
    std::vector<cache_entry> cache;
};

}  // namespace beta

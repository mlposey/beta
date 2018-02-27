#pragma once
#include <memory>
#include "path/shareablepath.h"

namespace beta {

/** Pairs a path query with the path that answers it */
struct cache_entry {
    /** Returns the expected size of the entry in bytes */
    size_t sizeBytes() const {
        return PATH_QUERY_SIZE_BYTES + path->nodeCount() * NODE_SIZE_BYTES;
    }
    path_query query;
    std::shared_ptr<Path> path;
};

}  // namespace beta
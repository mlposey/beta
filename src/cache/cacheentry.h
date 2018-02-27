#pragma once
#include <memory>
#include "path/shareablepath.h"

namespace beta {

struct cache_entry {
    size_t sizeBytes() const {
        return PATH_QUERY_SIZE_BYTES + path->nodeCount() * POINT_SIZE_BYTES;
    }
    path_query query;
    std::shared_ptr<Path> path;
};

}  // namespace beta
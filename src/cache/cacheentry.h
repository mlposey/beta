#pragma once
#include <memory>
#include "path/shareablepath.h"

namespace beta {

/** Pairs a path query with the path that answers it */
class CacheEntry {
public:
    CacheEntry(std::shared_ptr<Path> path) : path_(path) {}

    /** Returns a query containing the entry origin and destination */
    path_query query() const {
        return {*path_->begin(), *(path_->end() - 1)};
    }

    std::shared_ptr<Path> path() const {
        return path_;
    }

    /** Returns the estimated size of the entry in bytes */
    size_t sizeBytes() const {
        return sizeof(path_) + path_->nodeCount() * NODE_SIZE_BYTES;
    }

private:
    std::shared_ptr<Path> path_;
};

}  // namespace beta
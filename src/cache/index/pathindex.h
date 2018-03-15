#pragma once
#include <memory>
#include "path/path.h"
#include "path/pathquery.h"

namespace beta {

/** Describes a fast access type used to locate paths */
struct PathIndex {
    virtual ~PathIndex() = default;
    virtual void add(std::shared_ptr<Path> path) = 0;
    virtual void remove(std::shared_ptr<Path> path) = 0;
    virtual std::shared_ptr<Path> find(const path_query &query) const = 0;
    virtual size_t sizeBytes(std::shared_ptr<Path> path) const = 0;
};

}  // namespace beta
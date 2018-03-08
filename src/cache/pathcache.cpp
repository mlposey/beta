#include <stdexcept>
#include "pathcache.h"
#include "cache/optimize/optimizer.h"

namespace beta {

PathCache::PathCache(size_t capacityBytes)
    : PathCache(nullptr, capacityBytes)
{}

PathCache::PathCache(std::vector<std::shared_ptr<Path>> *paths, size_t capacityBytes)
    : capacityBytes(capacityBytes)
    , currentSizeBytes(0)
    , pathIndex(std::make_shared<PathRTree>()) {
    if (capacityBytes == 0) throw std::runtime_error("path cache size must be > 0");

    if (paths) {
        for (auto &path : *paths) {
            add(path);
        }
    }
}

void PathCache::add(std::shared_ptr<Path> path) {
    CacheEntry entry(path);

    size_t newSize = currentSizeBytes + entry.sizeBytes() + pathIndex->sizeBytes(path);
    if (newSize >= capacityBytes) {
        optimizeCache();
        if (currentSizeBytes >= capacityBytes) {
            throw std::overflow_error("memory usage exceeded limit " + std::to_string(capacityBytes));            
        }
    } else currentSizeBytes = newSize;

    cache.push_back(entry);
    pathIndex->add(path);
}

void PathCache::optimizeCache() {
    puts("optimizing cache...");
    printf("starting mem usage - %.2fMB\n", currentSizeBytes / (float) 1'000'000);
    Optimizer optimizer(cache, pathIndex);
    optimizer.optimize(capacityBytes, currentSizeBytes);
    printf("ending mem usage - %.2fMB\n", currentSizeBytes / (float) 1'000'000);
    puts("finished optimization");
}

}  // namespace beta

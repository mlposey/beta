#include <algorithm>
#include "optimizer.h"

namespace beta {

Optimizer::Optimizer(std::list<CacheEntry> &cache, std::shared_ptr<PathIndex> index)
    : cache(cache)
    , index(index)
{}

void Optimizer::optimize(size_t capacityBytes, size_t &currentSizeBytes) {
    sortCacheByRealDistDesc();
    removeSubpaths();
    sortCacheBySAPNDesc();
    prune(capacityBytes, currentSizeBytes);
}

void Optimizer::sortCacheByRealDistDesc() {
    cache.sort([](const auto &a, const auto &b) {
        return a.path()->realDistance() > b.path()->realDistance();
    });
}

void Optimizer::sortCacheBySAPNDesc() {
    cache.sort([&](const auto &a, const auto &b) {
        return pathRanks[a.path().get()] > pathRanks[b.path().get()];
    });
}

void Optimizer::removeSubpaths() {
    for (auto it = cache.begin(); it != cache.end(); ++it) {
        ShareablePath path(it->path());
        size_t sharingAbility = 1;

        for (auto jit = std::next(it); jit != cache.end();) {
            if (path.canShare(jit->query())) {
                sharingAbility++;
                index->remove(jit->path());
                jit = cache.erase(jit);
            } else {
                ++jit;
            }
        }
        pathRanks[it->path().get()] = (float) sharingAbility / it->path()->nodeCount();
    }
}

void Optimizer::prune(size_t capacityBytes, size_t &currentSizeBytes) {
    currentSizeBytes = 0;
    for (auto it = cache.begin(); it != cache.end(); ++it) {
        size_t cost = it->sizeBytes() + index->sizeBytes(it->path());
        if (currentSizeBytes + cost < capacityBytes) {
            currentSizeBytes += cost;
        } else {
            cache.erase(it, cache.end());
            return;
        }
    }
}

}  // namespace beta

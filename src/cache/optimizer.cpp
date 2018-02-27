#include <algorithm>
#include "optimizer.h"

namespace beta {

Optimizer::Optimizer(std::vector<cache_entry> &cache) : cache(cache) {}

size_t Optimizer::optimize(size_t capacityBytes) {
    this->capacityBytes = capacityBytes;
    sortCacheByRealDistDesc();

    std::vector<ranked_entry> uniqueEntries = rankCacheEntries();
    sortBySAPNDesc(uniqueEntries);
    keepMaxAllowed(uniqueEntries);
    return currentSizeBytes;
}

void Optimizer::sortCacheByRealDistDesc() {
    std::sort(cache.begin(), cache.end(), [](const auto &a, const auto &b) {
        return a.path->realDistance() > b.path->realDistance();
    });
}

std::vector<Optimizer::ranked_entry> Optimizer::rankCacheEntries() {
    std::vector<ranked_entry> unique;
    std::vector<bool> subpaths(cache.size(), false);

    for (int i = 0; i < cache.size(); i++) {
        while (subpaths[i]) i++;
        if (i == cache.size()) break;
        size_t sharingAbility = 1;
        ShareablePath path(cache[i].path);
        
        for (int j = i + 1; j < cache.size(); j++) {
            if (!subpaths[j] && path.canShare(cache[j].query)) {
                subpaths[j] = true;
                sharingAbility++;
            }
        }
        double saPerNode = (double) sharingAbility / cache[i].path->nodeCount();        
        unique.push_back({&cache[i], saPerNode});
    }
    return unique;
}

void Optimizer::sortBySAPNDesc(std::vector<ranked_entry> &rankedEntries) {
    std::sort(rankedEntries.begin(), rankedEntries.end(), [](const auto &a, const auto &b) {
        return a.sharingAbilityPerNode > b.sharingAbilityPerNode;
    });
}

void Optimizer::keepMaxAllowed(std::vector<ranked_entry> &rankedEntries) {
    currentSizeBytes = 0;
    std::vector<cache_entry> newCache;

    for (const auto &ranked : rankedEntries) {
        currentSizeBytes += ranked.entry->sizeBytes();

        if (currentSizeBytes <= capacityBytes) {
            newCache.push_back(*ranked.entry);
        } else {
            break;
        }
    }
    cache = newCache;
}

}  // namespace beta

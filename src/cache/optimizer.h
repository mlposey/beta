#pragma once
#include <vector>
#include "cacheentry.h"

namespace beta {

class Optimizer {
public:
    explicit Optimizer(std::vector<cache_entry> &cache);
    size_t optimize(size_t capacityBytes);

private:
    struct ranked_entry;

    std::vector<ranked_entry> rankCacheEntries();
    void keepMaxAllowed(std::vector<ranked_entry> &rankedEntries);

    void sortCacheByRealDistDesc();
    void sortBySAPNDesc(std::vector<ranked_entry> &rankedEntries);

    size_t capacityBytes;
    size_t currentSizeBytes;
    std::vector<cache_entry> &cache;
};

struct Optimizer::ranked_entry {
    cache_entry *entry;
    double sharingAbilityPerNode;
};

}  // namespace beta

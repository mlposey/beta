#include "config/config.h"
#include "cache/pathcache.h"
#include "net/server.h"
#include "version.h"

using namespace beta;

int main() {
    printf("BeTA v%s\n", APP_VERSION);

    // TODO: Load entries from disk if needed.
    std::vector<cache_entry> entries;
    PathCache cache(entries, 1000);

    // TODO: Load provider if needed.
    Server server(&cache, nullptr);
    server.listen(Config::getInstance().get("port"));
    return 0;
}

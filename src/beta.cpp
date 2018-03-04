#include "config/config.h"
#include "cache/pathcache.h"
#include "net/server.h"
#include "version.h"

using namespace beta;

int main() {
    printf("BeTA v%s\n", APP_VERSION);
    Config &config = Config::getInstance();

    // TODO: Load entries from disk if needed.
    PathCache cache(config.getBytes("max_cache_mem"));

    // TODO: Load provider if needed.
    Server server(&cache, nullptr);
    server.listen(config.getString("port"));
    return 0;
}

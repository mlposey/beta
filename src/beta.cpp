#include "config/config.h"
#include "cache/pathcache.h"
#include "provider/providerloader.h"
#include "net/server.h"
#include "flag/boostflagparser.h"
#include "version.h"

using namespace beta;

/** Loads a provider or defaults to nullptr if it cannot be found or was not requested */
std::shared_ptr<ProviderAdapter> loadProvider(FlagParser *flagParser);

int main(int argc, char **argv) {
    printf("BeTA v%s\n", APP_VERSION);

    BoostFlagParser flagParser;
    flagParser.parse(argc, argv);
    if (flagParser.wantsHelp()) {
        flagParser.showHelp();
        return 0;
    }

    Config &config = Config::getInstance();
    PathCache cache(config.getBytes("max_cache_mem"));
    
    Server server(std::move(cache), loadProvider(&flagParser));
    server.listen(config.getString("port"));
    return 0;
}

std::shared_ptr<ProviderAdapter> loadProvider(FlagParser *flagParser) {
    std::shared_ptr<ProviderAdapter> provider = nullptr;
    try {
        ProviderLoader loader(Config::getInstance().getString("provider_dir"));
        std::string providerName = flagParser->provider();
        if (!providerName.empty()) {
            provider = loader.load(providerName + ".so");
        }
    } catch (std::exception &e) {
        std::cout << e.what() << '\n'
                  << "all path retrieval will default to GetRoute behavior\n";
    }
    return provider;
}

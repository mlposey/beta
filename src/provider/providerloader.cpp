#include <cstdlib>
#include <stdexcept>
#include "providerloader.h"

namespace beta {

ProviderLoader::ProviderLoader(std::string providersDir)
    : providersDir(providersDir)
{}

std::shared_ptr<PathProvider> ProviderLoader::load(std::string filename) {
    std::shared_ptr<PathProvider> provider;
    try {
        boost::shared_ptr<PathProvider> tmp = importProvider(filename);
        provider = getStdHandle(tmp);
        verify(provider);
    } catch (std::exception &ex) {
        printf("failed to load provider %s\n", filename.c_str());
        printf("reason: %s\n", ex.what());
        throw ex;
    }
    return provider;
}

boost::shared_ptr<PathProvider> ProviderLoader::importProvider(std::string filename) {
    return boost::dll::import<PathProvider>(
        providersDir / filename,
        "provider",
        boost::dll::load_mode::append_decorations
    );
}

std::shared_ptr<PathProvider> ProviderLoader::getStdHandle(boost::shared_ptr<PathProvider> provider) {
    return std::shared_ptr<PathProvider>(provider.get(), [provider](...) mutable { provider.reset(); });
}

void ProviderLoader::verify(std::shared_ptr<PathProvider> provider) {
    if (provider->name().empty()) {
        throw std::runtime_error("missing provider name");
    }
    if (provider->author().empty()) {
        throw std::runtime_error("missing provider author");
    }
}

}  // namespace beta

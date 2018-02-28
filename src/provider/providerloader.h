#pragma once
#include <memory>
#include <boost/dll/import.hpp>
#include "pathprovider.h"

namespace beta {

/** Loads provider libraries into the application */
class ProviderLoader {
public:
    /** Creates a ProviderLoader that searches a given directory for providers */
    ProviderLoader(std::string providersDir);

    /**
     * Loads a provider from the providers directory
     * 
     * @param filename the name of the shared library file, most likely having a .so extension
     * @throws std::runtime_error if the provider could not be loaded
     */
    std::shared_ptr<PathProvider> load(std::string filename);

private:
    boost::shared_ptr<PathProvider> importProvider(std::string filename);
    std::shared_ptr<PathProvider> getStdHandle(boost::shared_ptr<PathProvider> provider);
    void verify(std::shared_ptr<PathProvider> provider);

    boost::filesystem::path providersDir;
};

}  // namespace beta

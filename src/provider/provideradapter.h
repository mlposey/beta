#pragma once
#include <memory>
#include <vector>
#include "pathprovider.h"
#include "path/path.h"
#include "path/pathquery.h"

namespace beta {

/**
 * Provides expected data types to the core codebase and PathProviders
 * 
 * The data types that PathProvider uses are much simpler than what the
 * main codebase uses. That is intentional; using standard library types
 * ensures the API remains stable, while also reducing the complexity
 * that provider developers face. This class allows the simplified code
 * and the domain code to communicate using the same "language".
 */
class ProviderAdapter {
public:
    ProviderAdapter(std::shared_ptr<PathProvider> provider);

    /**
     * Sends a query request to the provider
     * @returns the path that answers the query or nullptr if no path exists
     */
    std::shared_ptr<Path> findPath(path_query query);

    std::shared_ptr<PathProvider> provider() const { return provider_; }

private:
    std::vector<float> invokeProvider(path_query &query);
    std::shared_ptr<Path> convert(const std::vector<float> &rawPath);

    std::shared_ptr<PathProvider> provider_;
};

}  // namespace beta
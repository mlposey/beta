#include "provideradapter.h"
#include "path/path2d.h"

namespace beta {

ProviderAdapter::ProviderAdapter(std::shared_ptr<PathProvider> provider)
    : provider_(provider)
{}

std::shared_ptr<Path> ProviderAdapter::findPath(path_query query) {
    std::vector<float> rawPath = invokeProvider(query);
    return convert(rawPath);
}

std::vector<float> ProviderAdapter::invokeProvider(path_query &query) {
    return provider_->findPath(
        query.origin.latitude(),
        query.origin.longitude(),
        query.destination.latitude(),
        query.destination.longitude()
    );
}

std::shared_ptr<Path> ProviderAdapter::convert(const std::vector<float> &rawPath) {
    const size_t coordCount = rawPath.size();
    if (coordCount < 4 || coordCount % 2 != 0) return nullptr;
    
    auto path = std::make_shared<Path2D>();
    for (size_t i = 0; i < coordCount - 1; ++i) {
        path->push_back(Node(rawPath[i], rawPath[i + 1]));
    }
    return path;
}

}  // namespace beta
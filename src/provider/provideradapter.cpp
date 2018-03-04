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
    
    Node a(rawPath[0], rawPath[1]), b(rawPath[2], rawPath[3]);
    std::shared_ptr<Path> path = std::make_shared<Path2D>(a, b);
    for (int i = 4; i < coordCount - 1; i++) {
        path->add(Node(rawPath[i], rawPath[i + 1]));
    }
    return path;
}

}  // namespace beta
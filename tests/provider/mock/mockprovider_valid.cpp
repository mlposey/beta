#include <boost/dll/alias.hpp>
#include "pathprovider.h"

class MockProvider : public PathProvider {
public:
    std::string name() const { return "MockProvider"; }
    float version() const { return 0.1; }
    std::string author() const { return "Mock"; }

    std::vector<float> findPath(float srcLat, float srcLng, float dstLat, float dstLng) {
        return {3.0, 4.0};
    }
};

extern "C" BOOST_SYMBOL_EXPORT MockProvider provider;
MockProvider provider;

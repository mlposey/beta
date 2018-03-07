#include <cmath>
#include <sstream>
#include "node.h"

namespace beta {

Node::Node(float latitude, float longitude)
    : lat(latitude)
    , lng(longitude)
{}

double Node::distanceTo(const Node &destination) const {
    double latDiff = destination.latitude() - latitude();
    double lngDiff = destination.longitude() - longitude();
    return sqrt(pow(latDiff, 2) + pow(lngDiff, 2));
}

bool Node::isBetween(const Node &a, const Node &b) const {
    float l = (latitude() - a.latitude()) / (b.latitude() - a.latitude());
    // TODO: This delta may be too large.
    return abs(l - (longitude() - a.longitude()) / (b.longitude() - a.longitude())) < 0.01 && l >= 0 && l <= 1;
}
    
bool Node::operator==(const Node &other) const {
    return latitude() == other.latitude() && lng == other.longitude();
}

std::string Node::str() const {
    std::stringstream ss;
    ss << "(" << latitude() << "," << longitude() << ")";
    return ss.str();
}

}  // namespace beta
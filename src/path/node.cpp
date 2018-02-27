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

double Node::isBetween(const Node &a, const Node &b) const {
    return a.distanceTo(b) == a.distanceTo(*this) + this->distanceTo(b);
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
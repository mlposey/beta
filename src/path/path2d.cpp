#include <cmath>
#include "path2d.h"

namespace beta {

Path2D::Path2D(const Node &origin, const Node &destination) {
    nodes.emplace_back(origin);
    nodes.emplace_back(destination);
    realDist = origin.distanceTo(destination);
}

void Path2D::add(const Node &node) {
    realDist += nodes.back().distanceTo(node);
    nodes.emplace_back(node);
}

bool Path2D::contains(const Node &node) const {
    for (size_t i = 0; i < nodes.size() - 1; i++) {
        if (node.isBetween(nodes[i], nodes[i+1])) {
            return true;
        }
    }
    return false;
}

size_t Path2D::nodeCount() const {
    return nodes.size();
}

size_t Path2D::sizeBytes() const {
    return sizeof(*this) + nodes.size() * sizeof(Node);
}

double Path2D::euclideanDistance() const {
    return nodes[0].distanceTo(nodes.back());
}

double Path2D::realDistance() const {
    return realDist;
}

std::string Path2D::str() const {
    return "[" + nodes[0].str() + " -> " + nodes.back().str() + "]";
}


}  // namespace beta

#include <cmath>
#include "path2d.h"

namespace beta {

Path2D::Path2D(const point &origin, const point &destination) {
    points.emplace_back(origin);
    points.emplace_back(destination);
    realDist = origin.distanceTo(destination);
}

void Path2D::add(const point &point) {
    realDist += points.back().distanceTo(point);
    points.emplace_back(point);
}

bool Path2D::contains(const point &p) const {
    for (int i = 0; i < points.size() - 1; i++) {
        if (p.isBetween(points[i], points[i+1])) {
            return true;
        }
    }
    return false;
}

size_t Path2D::nodeCount() const {
    return points.size();
}

double Path2D::euclideanDistance() const {
    return points[0].distanceTo(points.back());
}

double Path2D::realDistance() const {
    return realDist;
}

std::string Path2D::toString() const {
    return "[" + points[0].toString() + " -> " + points.back().toString() + "]";
}


}  // namespace beta

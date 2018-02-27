#pragma once

#include <vector>
#include "path.h"

namespace beta {

class Path2D : public Path {
public:
    Path2D(const point &origin, const point &destination);

    void add(const point &point) override;
    bool contains(const point &p) const override;

    const_iterator begin() override { return &points[0]; }
    const_iterator end() override { return &points[points.size()]; }
    
    size_t nodeCount() const override;
    double euclideanDistance() const override;
    double realDistance() const override;

    std::string toString() const override;

private:
    std::vector<point> points;
    double realDist;
};

}  // namespace beta

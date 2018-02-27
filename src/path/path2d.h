#pragma once

#include <vector>
#include "path.h"

namespace beta {

class Path2D : public Path {
public:
    Path2D(const Node &origin, const Node &destination);

    void add(const Node &node) override;
    bool contains(const Node &node) const override;

    const_iterator begin() override { return &nodes[0]; }
    const_iterator end() override { return &nodes[nodes.size()]; }
    
    size_t nodeCount() const override;
    double euclideanDistance() const override;
    double realDistance() const override;

    std::string str() const override;

private:
    std::vector<Node> nodes;
    double realDist;
};

}  // namespace beta

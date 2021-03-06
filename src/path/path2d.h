#pragma once
#include <deque>
#include "path.h"

namespace beta {

/** Defines a path of connected nodes in a two-dimensional space */
class Path2D : public Path {
public:
    Path2D() = default;
    /** Creates a Path2D instance with an initial two-node path */
    Path2D(const Node &origin, const Node &destination);

    /**
     * Pushes a node to the end of the path
     * 
     * By appending a node to the path, a new destination is created. This
     * may alter the existing real and euclidean distances.
     */
    void push_back(const Node &node) override;

    /**
     * Pushes a node to the beginning of the path
     * 
     * By prepending a node to the path, a new origin is created. This
     * may alter the existing real and euclidean distances.
     */
    void push_front(const Node &node) override;

    /** Determines if a node sits directly on any line of the path */
    bool contains(const Node &node) const override;

    /** Returns an iterator to the beginning of the path */
    iterator begin() override { return nodes.begin(); }
    /** Returns an iterator to a position one element past the path end */
    iterator end() override { return nodes.end(); }
    
    /** Returns the number of nodes in the path */
    size_t nodeCount() const override;
    /** Returns the size of the path in bytes */
    size_t sizeBytes() const override;
    /** Returns the straight line distance from the path's origin to its destination */
    double euclideanDistance() const override;
    /** Returns the total distance of a each step in the path */
    double realDistance() const override;

    /** Returns a shorthand string representation of the path */
    std::string str() const override;

private:
    std::deque<Node> nodes;
    double realDist;
};

}  // namespace beta

#pragma once
#include <cstddef>
#include <deque>
#include "node.h"

namespace beta {

/** Describes an ordered collection of connected Nodes */
struct Path {
    virtual ~Path() = default;

    virtual void push_back(const Node &node) = 0;
    virtual void push_front(const Node &node) = 0;
    virtual bool contains(const Node &node) const = 0;
    
    using const_iterator = std::deque<Node>::iterator;
    virtual const_iterator begin() = 0;
    virtual const_iterator end() = 0;
    
    virtual size_t nodeCount() const = 0;
    virtual size_t sizeBytes() const = 0;

    virtual double euclideanDistance() const = 0;
    virtual double realDistance() const = 0;

    virtual std::string str() const = 0;
};

}  // namespace beta

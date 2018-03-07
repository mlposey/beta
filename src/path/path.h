#pragma once
#include <cstddef>
#include "node.h"

namespace beta {

/** Describes an ordered collection of connected Nodes */
struct Path {
    virtual ~Path() = default;

    virtual void add(const Node &node) = 0;
    virtual bool contains(const Node &node) const = 0;
    
    using const_iterator = const Node*;
    virtual const_iterator begin() = 0;
    virtual const_iterator end() = 0;
    
    virtual size_t nodeCount() const = 0;
    virtual size_t sizeBytes() const = 0;

    virtual double euclideanDistance() const = 0;
    virtual double realDistance() const = 0;

    virtual std::string str() const = 0;
};

}  // namespace beta

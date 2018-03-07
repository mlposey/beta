#pragma once
#include <string>
#include "node.h"

namespace beta {

/** Defines a request for a path using its starting and ending nodes */
struct path_query {
    /** Returns a string representation of the query */
    std::string str() const {
        return "[" + origin.str() + ", " + destination.str() + "]";
    }
    Node origin;
    Node destination;
};

// The expected size of a path_query object in bytes
#define PATH_QUERY_SIZE_BYTES NODE_SIZE_BYTES * 2

}  // namespace beta

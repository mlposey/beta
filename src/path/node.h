#pragma once
#include <string>

namespace beta {  

/** Defines a geographical point with a latitude and longitude */
class Node {
public:
    Node(float latitude, float longitude);

    /** Computes the euclidean distance to the destination node */
    double distanceTo(const Node &destination) const;
    /** Determines if the node sits on the straight line between a and b */
    bool isBetween(const Node &a, const Node &b) const;
    
    /** Determines if the nodes have equal coordinates */
    bool operator==(const Node &other) const;
    /** Returns a string representation of the node */
    std::string str() const;

    float latitude() const { return lat; }
    float longitude() const { return lng; }

private:
    float lat;
    float lng;
};

}  // namespace beta
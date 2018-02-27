#pragma once
#include <string>

namespace beta {

#define NODE_SIZE_BYTES 8    

class Node {
public:
    Node(float latitude, float longitude);

    double distanceTo(const Node &destination) const;
    double isBetween(const Node &a, const Node &b) const;
    
    bool operator==(const Node &other) const;
    std::string str() const;

    float latitude() const { return lat; }
    float longitude() const { return lng; }

private:
    float lat;
    float lng;
};

}  // namespace beta
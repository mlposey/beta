#pragma once

#include <cstddef>
#include <cmath>
#include <functional>
#include <string>
#include <sstream>

namespace beta {

// TODO: This should be an interface.
struct point {
    double distanceTo(const point &destination) const {
        double latDiff = destination.lat - lat;
        double lngDiff = destination.lng - lng;
        return sqrt(pow(latDiff, 2) + pow(lngDiff, 2));
    }

    double isBetween(const point &a, const point &b) const {
        return a.distanceTo(b) == a.distanceTo(*this) + this->distanceTo(b);
    }
    
    bool operator==(const point &other) const {
        return lat == other.lat && lng == other.lng;
    }

    std::string toString() const {
        std::stringstream ss;
        ss << "(" << lat << "," << lng << ")";
        return ss.str();
    }

    float lat;
    float lng;
};

#define POINT_SIZE_BYTES 8

struct Path {
    virtual ~Path() = default;

    virtual void add(const point &p) = 0;
    virtual bool contains(const point &p) const = 0;
    
    using const_iterator = const point*;
    virtual const_iterator begin() = 0;
    virtual const_iterator end() = 0;
    
    virtual size_t nodeCount() const = 0;

    virtual double euclideanDistance() const = 0;
    virtual double realDistance() const = 0;

    virtual std::string toString() const = 0;
};

}  // namespace beta

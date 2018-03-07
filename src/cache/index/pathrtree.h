#pragma once
#include <functional>
#include <boost/geometry.hpp>
#include <boost/geometry/geometries/register/point.hpp>
#include <boost/geometry/geometries/register/box.hpp>
#include <boost/geometry/index/rtree.hpp>
#include "pathindex.h"
#include "path/path.h"

namespace beta {

namespace bg = boost::geometry;
namespace bgi = boost::geometry::index;

// Defines a point in two-dimensional space
using point = bg::model::point<float, 2, bg::cs::cartesian>;
/** Makes a point using the latitude and longitude of a Node */
inline point makePoint(const Node &n) {
    return point(n.latitude(), n.longitude());
}

// Defines a line segment or step from one point in a path to another
using segment = bg::model::segment<point>;
// Maps a segment to the path in which it belongs
using path_ref = std::pair<segment, std::shared_ptr<Path>>;

/**
 * Indexes paths in a R-tree
 * 
 * PathRTree places each segment of a path into an R-tree and uses point
 * intersections to determine if queries can be answered.
 */
class PathRTree : public PathIndex {
public:
    /** Adds a path to the tree */
    void add(std::shared_ptr<Path> path) override;

    /** Removes a single matching path from the tree */
    void remove(std::shared_ptr<Path> path) override;

    /**
     * Finds a path whose origin and destination match the query
     * @return the matching path or nullptr if none exists
     */
    std::shared_ptr<Path> find(const path_query &query) const override;

    /** Determines if the tree contains a path that matches the argument exactly */
    bool contains(std::shared_ptr<Path> path) const override;
    /** Determines if the tree contains a path that contains the query origin and destination */
    bool contains(const path_query &query) const override;

    /** Returns the size that a given path would consume in the tree */
    size_t sizeBytes(std::shared_ptr<Path> path) const override;

private:
    void handleIntersectingPaths(point p, std::function<void(std::shared_ptr<Path>)> fn) const;

    bgi::rtree<path_ref, bgi::quadratic<16>> tree;
};

}  // namespace beta

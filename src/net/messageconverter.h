#pragma once
#include "path/node.h"
#include "path/path2d.h"
#include "path/pathquery.h"
#include "protobuf/beta.grpc.pb.h"

namespace beta {

/** Converts between protocol buffer messages and domain types */
class MessageConverter {
public:
    /**
     * Converts a protobuf Route to a beta::Path
     * @returns nullptr if the route does not contain at least two nodes
     */
    static std::shared_ptr<Path> convert(const betagrpc::Route *route);
    /** Converts a protobuf Node to a beta::Node */
    static Node convert(const betagrpc::Node &node);
    /** Converts a beta::Path to a protobuf Route */
    static void convert(std::shared_ptr<Path> src, betagrpc::Route *dst);
    /** Converts a protobuf RouteQuery to a beta::path_query */
    static path_query convert(const betagrpc::RouteQuery *query);
};

}  // namespace beta

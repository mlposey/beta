#pragma once
#include "path/node.h"
#include "path/path2d.h"
#include "path/pathquery.h"
#include "protobuf/beta.grpc.pb.h"

namespace beta {

/** Converts between protocol buffer messages and domain types */
class MessageConverter {
public:
    /** @returns nullptr if the route does not contain at least two nodes */
    static std::shared_ptr<Path> convert(const betagrpc::Route *route);
    static Node convert(const betagrpc::Node &node);
    static void convert(std::shared_ptr<Path> src, betagrpc::Route *dst);
    static path_query convert(const betagrpc::RouteQuery *query);
};

}  // namespace beta

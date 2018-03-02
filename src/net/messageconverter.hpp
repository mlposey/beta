#pragma once
#include "path/node.h"
#include "path/path2d.h"
#include "protobuf/beta.grpc.pb.h"

namespace beta {

/** Converts between protocol buffer messages and domain types */
class MessageConverter {
public:
    static Node convert(const betagrpc::Node &node);
    /** @returns nullptr if the route does not contain at least two nodes */
    static std::shared_ptr<Path> convert(const betagrpc::Route *route);
    static void convert(std::shared_ptr<Path> src, betagrpc::Route *dst);
};

Node MessageConverter::convert(const betagrpc::Node &node) {
    return {node.latitude(), node.longitude()};
}

std::shared_ptr<Path> MessageConverter::convert(const betagrpc::Route *route) {
    const int routeLength = route->nodes_size();
    if (routeLength < 2) {
        return nullptr;
    }
    Node a = MessageConverter::convert(route->nodes(0));
    Node b = MessageConverter::convert(route->nodes(1));
    std::shared_ptr<Path> path = std::make_shared<Path2D>(a, b);

    for (int i = 2; i < routeLength; i++) {
        path->add(MessageConverter::convert(route->nodes(i)));
    }
    return path;
}

void MessageConverter::convert(std::shared_ptr<Path> src, betagrpc::Route *dst) {
    dst->set_distance(src->realDistance());
    for (auto it = src->begin(); it != src->end(); it++) {
        betagrpc::Node *node = dst->add_nodes();
        node->set_latitude(it->latitude());
        node->set_longitude(it->longitude());
    }
}

}  // namespace beta

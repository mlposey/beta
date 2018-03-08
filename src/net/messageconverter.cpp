#include "messageconverter.h"

namespace beta {

Node MessageConverter::convert(const betagrpc::Node &node) {
    return Node(node.latitude(), node.longitude());
}

std::shared_ptr<Path> MessageConverter::convert(const betagrpc::Route *route) {
    const int routeLength = route->nodes_size();
    if (routeLength < 2) return nullptr;
    
    auto path = std::make_shared<Path2D>();
    for (int i = 0; i < routeLength; ++i) {
        path->push_back(MessageConverter::convert(route->nodes(i)));
    }
    return path;
}

void MessageConverter::convert(std::shared_ptr<Path> src, betagrpc::Route *dst) {
    dst->set_distance(src->realDistance());
    for (auto it = src->begin(); it != src->end(); ++it) {
        betagrpc::Node *node = dst->add_nodes();
        node->set_latitude(it->latitude());
        node->set_longitude(it->longitude());
    }
}

path_query MessageConverter::convert(const betagrpc::RouteQuery *query) {
    return {
        MessageConverter::convert(query->origin()),
        MessageConverter::convert(query->destination())
    };
}

}  // namespace beta

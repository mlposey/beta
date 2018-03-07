#include "server.h"
#include "path/path2d.h"
#include "messageconverter.h"

namespace beta {

Server::Server(PathCache *cache, std::shared_ptr<ProviderAdapter> provider)
    : cache(cache)
    , provider(provider)
{}

void Server::listen(std::string port) {
    std::string listenAddr = "0.0.0.0:" + port;

    ServerBuilder builder;
    builder.AddListeningPort(listenAddr, grpc::InsecureServerCredentials());
    builder.RegisterService(this);

    std::unique_ptr<grpc::Server> server(builder.BuildAndStart());
    std::cout << "listening on " << listenAddr << '\n';
    server->Wait();
}

Status Server::GetRoute(ServerContext *ctx, const RouteQuery *query, Route *route) {
    auto path = cache->find(MessageConverter::convert(query));
    if (path) MessageConverter::convert(path, route);
    return Status::OK;
}

Status Server::MustGetRoute(ServerContext *ctx, const RouteQuery *query, Route *route) {
    GetRoute(ctx, query, route);
    if (route->nodes_size() == 0 && provider) {
        std::shared_ptr<Path> path = provider->findPath(MessageConverter::convert(query));
        if (path) {
            cache->add(path);
            MessageConverter::convert(path, route);
        }
    }
    return Status::OK;
}

Status Server::AddRoute(ServerContext *ctx, const Route *route, AddResponse *res) {
    std::shared_ptr<Path> path = MessageConverter::convert(route);
    if (!path) return Status::CANCELLED;
    cache->add(path);
    return Status::OK;
}

}  // namespace beta

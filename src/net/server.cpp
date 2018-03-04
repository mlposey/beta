#include "server.h"
#include "path/path2d.h"
#include "messageconverter.hpp"

namespace beta {

Server::Server(PathCache *cache, std::shared_ptr<PathProvider> provider)
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
    printf("getting route for client...\n");

    auto path = cache->find(MessageConverter::convert(query));
    if (path) {
        printf("found route %s\nbuilding response...\n", path->str().c_str());
        MessageConverter::convert(path, route);
    } else {
        printf("could not find path\n");
    }

    printf("done.\n");    
    return Status::OK;
}

Status Server::MustGetRoute(ServerContext *ctx, const RouteQuery *query, Route *route) {
    // TODO: MustGetRoute
    return Status::OK;
}

Status Server::AddRoute(ServerContext *ctx, const Route *route, AddResponse *res) {
    printf("adding route for client...\n");
    std::shared_ptr<Path> path = MessageConverter::convert(route);
    if (!path) {
        return Status::CANCELLED;
    }
    printf("built path %s for client\n", path->str().c_str());
    
    cache->add(path);
    printf("done.\n");
    return Status::OK;
}

}  // namespace beta

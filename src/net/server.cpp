#include "server.h"

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
    // TODO: GetRoute
    return Status::OK;
}

Status Server::MustGetRoute(ServerContext *ctx, const RouteQuery *query, Route *route) {
    // TODO: MustGetRoute
    return Status::OK;
}

Status Server::AddRoute(ServerContext *ctx, const Route *route, AddResponse *res) {
    // TODO: AddRoute
    return Status::OK;
}

}  // namespace beta
